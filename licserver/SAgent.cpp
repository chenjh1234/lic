#include "SAgent.h"
SAgent::SAgent()
{
    init();

//    setIP(PORTAL_IP);
 //   setPort(QString(PORTAL_PORT).toShort());

}
SAgent::~ SAgent()
{


}
void SAgent::init()
{
   _config.setDbFile(AGENT_CONFIG_FILE);
   setDbFile(AGENT_DB_FILE);
   setLogFile(AGENT_LOG_FILE);
   _log.setFile(logFile());

}
#if 0
int SAgent::httpLogin(SAppInfo& msg)
{
   Json::Value params, result;
   string ip = _IP.Q2CH;
   unsigned short port;
   port = _port;
   HttpClient hc(ip, port);
   hc.init();
   params = toJason(msg);
   bool b = hc.Call(LIC_URL, CMD_LOGINAPP, params, result);
   hc.stop();
   if (b)
   {
      if (getJsonV(result, RET_STATUS) == QString(RET_OK))
      {
         return 1;
      }
      else
      {
         return -1;
      }
   }
   else return -1;
}
int SAgent::httpLogout(SAppInfo& msg)
{
   Json::Value params, result;
   string ip = _IP.Q2CH;
   unsigned short port;
   port = _port;
   HttpClient hc(ip, port);
   hc.init();
   bool b = hc.Call(LIC_URL, CMD_LOGOUTAPP, params, result);
   hc.stop();
   if (b)
   {
      if (getJsonV(result, RET_STATUS) == QString(RET_OK))
      {
         return 1;
      }
      else
      {
         return -1;
      }
   }
   else return -1;
}
//login logout
int SAgent::login(SAppInfo& msg)
{
   int i,ii;
   QString  err;
   i = -10;
   if (isLoginConfig(msg))  // if lgin config, we do login app;
   {
      i =  httpLogin(msg);
   }
   else
   {
      QString  appname = msg.get(APP_NAME).toString();
      // config limited:
      err =  msg.encodePackageID() + " --- " + appname + "_" +  msg.encodeAppID() + "--- config limited error";
      log(err);
      return i;
   }

   if (i <= 0) // if login config  and : no license space , we unlogin config
   {
      ii = isLogoutConfig(msg);
      if (ii < 0)   log("logout config error ", msg.encodePackageID(), " ", msg.encodeAppID());
   }
   else
   {
      i = registerApp(msg);
   }
   return i;
}
int SAgent::logout(SAppInfo& msg)
{
   int i;
   QString err;

   i =  httpLogout(msg);
   if (i >= 0)  // logout ok we unlogin config;
   {
      i = isLogoutConfig(msg);
      if (i < 0)   log("logout config error ", msg.encodePackageID(), " ", msg.encodeAppID());
      i = unRegisterApp(msg);
   }
   else
   {
      err = "httpLogout error " + msg.encodePackageID() + " " + msg.encodeAppID();
      log(err);
      i = isLogoutConfig(msg);
      if (i < 0)   log("logout config error ", msg.encodePackageID(), " ", msg.encodeAppID());
   }
   return i;
}
#endif
bool SAgent::isLoginConfig(SAppInfo& msg)
{
   SAppInfo *info;

   QString vender, package, version, ip, user, packid, node, licproject;
   int number, i;
   info = &msg;
   vender = info->get(APP_VENDER).toString();
   package = info->get(APP_PACKAGE).toString();
   version = info->get(APP_VERSION).toString();
   number = info->get(APP_NUMBER).toInt();
   ip = info->get(APP_IP).toString();
   user = info->get(APP_USER).toString();
   licproject  = info->get(APP_LICPROJECT).toString();
   //packid = data->encodePackageId(vender, package, version);
   packid = info->encodePackageID();

   node = ip;
   i = _config.login(packid, user, node, licproject, number);
   qDebug() << "isLoginConfig=" << packid << user << node << licproject << number << i;
   if (i == LOGIN_OK) return true;
   else return false;
}
bool SAgent::isLogoutConfig(SAppInfo& msg)
{
   SAppInfo *info;
   QString vender, package, version, ip, user, packid, node, licproject;
   int number, i;
   info = &msg;
   vender = info->get(APP_VENDER).toString();
   package = info->get(APP_PACKAGE).toString();
   version = info->get(APP_VERSION).toString();
   number = info->get(APP_NUMBER).toInt();
   ip = info->get(APP_IP).toString();
   user = info->get(APP_USER).toString();
   licproject  = info->get(APP_LICPROJECT).toString();
   // packid = data->encodePackageId(vender, package, version);
   packid = info->encodePackageID();

   node = ip;
   i = _config.unlogin(packid, user, node, licproject, number);
   if (i == LOGIN_OK) return true;
   else return false;
}

// login:
int SAgent::registerApp(SAppInfo& ainfo)
{
   int i, iret;
   LFileDate fd;
   QString str;
   QString vender, package, version, packid, pid, ip, user, appname;
   //SLicData sdata;

// check App info
   SAppInfo *info;
   info = new  SAppInfo();
   *info = ainfo;

   vender = info->get(APP_VENDER).toString();
   package = info->get(APP_PACKAGE).toString();
   version = info->get(APP_VERSION).toString();
   //number = info->get(APP_NUMBER).toInt();
   ip = info->get(APP_IP).toString();
   pid = info->get(APP_PID).toString();
   user = info->get(APP_USER).toString();
   //  appname = info->get(APP_NAME).toString();

   qDebug() << "info=" << info->getText();
   // qDebug() << "ainfo=";// << ainfo.getText();
   packid = info->encodePackageID();

   info->packid = packid;
   //info->number = number;
   info->appid = info->encodeAppID();
   info->user = user;
   info->nameid = info->appid;
   info->start = fd.sEP();
    qDebug() << "info apckid,appid = " << packid << info->appid;

   SAppMng *mng;
   mng = getAppMng(packid);

   _lockApp.lock();
   if (mng != NULL)
   {
      iret = mng->add(info);
   }
   _lockApp.unlock();

   if(iret >0 ) str = "registerApp OK ";
   else str = "registerApp Err ";
   str +=  packid + " " + info->appid;
    log(str);
   return iret;
}
int SAgent::unRegisterApp(SAppInfo& info)
{
   QString packid, appid;
   packid = info.encodePackageID();
   appid = info.encodeAppID();
   return unRegisterApp(packid, appid);
}
 
int SAgent::unRegisterApp(QString packid, QString appid)
{
   int iret;
   SAppMng *mng;
   SAppInfo *info;
   mng = NULL;
   qDebug() << "unregister ---" << packid << appid << isAppMng(packid);
    if (isAppMng(packid))
   {
      mng = packMng[packid];
      if (mng != NULL)
      {
         _lockApp.lock();
         info = (SAppInfo *)mng->get(appid);
         iret = mng->rmInfo(appid);
         qDebug() << "unregister ,rminfo,ret =  " << iret;
         if (iret >= 0 && info != NULL)  delete info;
         if (iret == 0)  // mng is empty;
         {
            iret = removePack(packid);
         }
         _lockApp.unlock();
      }
      else
      {
         iret = -2; //mng is NULL;
         qDebug() << "Error packid = " << packid << "  appmng is NULL ";
      }
   }
   else
   {
      iret = -1; // no this packid
      qDebug() << " Error No this package ID " << packid;
   }

   QString str;
   if(iret >0 ) str = "unregisterApp OK ";
   else str = "unregisterApp Err ";
   str +=  packid + " " + appid;
    log(str);
   return iret; // >0 normally

}


//======DB====================
int SAgent::saveDB()
{

   QString fileDB;
   QStringList slist;
   int iret;

   fileDB = dbFile();
   //qDebug() << " save app ok0=" << _dt.TID(fileDB);
   QFile file(fileDB);
   if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return -1;

   QDataStream ds(&file);
   iret = saveDBApp(ds);

    QString str;
   if(iret >=0 ) str = "saveDB OK ";
   else str = "saveDB Err ";
   //str +=  packid + " " + appid;
    log(str);
   return iret;
}
int SAgent::saveDBApp(QDataStream& ds)
{
   int sz, i;
   QStringList slist;

   SAppMng *amng;
   slist = packMng.keys();
   ds << slist;

   sz = slist.size();
   for (i = 0; i < sz; i++)
   {
      //  qDebug() << i << slist[i];
      amng = packMng[slist[i]];
      // qDebug() << i << slist[i] << amng;
      if (amng != NULL)  amng->encode(ds);
   }
   return sz;
}
int  SAgent::loadDB()
{
   int iret;
   QString str;
   QString fileDB;
   QStringList slist;

   fileDB = dbFile();
   QFile file(fileDB);
   if (!file.open(QIODevice::ReadOnly)) 
   {
       iret=0;// file.open(QIODevice::ReadWrite);
   }
   else
   {   
        QDataStream ds(&file);
        iret = loadDBApp(ds);
         file.close();
   }

// close
  
 
  if(iret >0 ) str = "loadDB OK ";
  else if (iret==0) str = "no DB loaded ";
   else str = "loadDB Err "; 
  qDebug() << str;
  //str +=  packid + " " + appid;
   log(str);
   return iret;
}
int  SAgent::loadDBApp(QDataStream& ds)
{
   int sz, i;
   QString fileDB;
   QStringList slist;
   packMng.clear();

// app:
   SAppMng *amng;
   ds >> slist;

   sz = slist.size();
   for (i = 0; i < sz; i++)
   {
      amng = new SAppMng();
      amng->decode(ds);
      packMng[slist[i]] = amng;
   }
   return sz;
}

//===================appHB===========================HB
int SAgent::appHB(SAppInfo& app)
{
   int  number;
   LFileDate fd;
   QString str;
   QString vender, package, version, packid, pid, ip, user;
   // check App info
   SAppInfo *info;
   SAppMng *mng;
   mng = NULL;

   info = &app;

   vender = info->get(APP_VENDER).toString();
   package = info->get(APP_PACKAGE).toString();
   version = info->get(APP_VERSION).toString();
   //number = info->get(APP_NUMBER).toInt();
   ip = info->get(APP_IP).toString();
   pid = info->get(APP_PID).toString();
   user = info->get(APP_USER).toString();

   //packid = encodePackageId(vender, package, version);
   packid = info->encodePackageID();

   mng = packMng[packid];
   QString appid;

   //appid = encodeAppId(ip, pid);
   appid = info->encodeAppID();

   info = NULL;

   if (mng != NULL)  info = (SAppInfo *)mng->get(appid);
   if (info != NULL)  return info->appHB();
   else return -1;
}
QString SAgent::view(QString pack)
{
    QStringList slist;
    if (!pack.isEmpty())  return viewPack(pack);
    slist = packMng.keys();
    int i;
    QString p,ret;
    ret = "";

    for (i = 0; i <slist.size();i++) 
    {
        p = slist[i];
        ret += viewPack(p);
    }
    return ret;
}
QString SAgent::viewPack(QString pack )
{
    QString ret,appid,number,user,appname,st;
    SAppMng *mng;
    SAppInfo *info;
    int len1,len2,len3,i;
    len1 = 10;
    len2 = 20;
    len3 = 25;
    mng = packMng[pack];
    if (mng == NULL)  return "";
    for (i = 0; i < mng->size(); i++) 
    {
        info = (SAppInfo *)mng->get(i);
        if (info == NULL)  continue;
        appid = info->encodeAppID();
        appname = info->get(APP_NAME).toString();
        number = info->get(APP_NUMBER).toString();
        //qDebug() << "number = " << number;
        user = info->get(APP_USER).toString();
        st =    QString("%1 %2 %3 %4 %5\n").arg(pack, len3).arg(appid, len2).arg(user, len1).arg(number, len1).arg(appname,len3);
        ret += st;
    }
    return ret;
     
}
 
//==================== aux:========
SAppMng* SAgent::getAppMng(QString packid)
{
   SAppMng  *mng;
   if (packMng.contains(packid))
   {
      mng = packMng[packid];
   }
   else
   {
      mng = new SAppMng();
      if (mng !=NULL)  packMng[packid] = mng;
   }
   return mng;
}
bool SAgent::isAppMng(QString packid)
{
   return packMng.contains(packid);
}

int SAgent::removePack(QString packid)
{
   return packMng.remove(packid);
}
// files================
QString SAgent::dbFile()
{
   return _dbFile;
}
void SAgent::setDbFile(QString file)
{
   _dbFile = file;
}

QString SAgent::logFile()
{
   return _logFile;
}
void SAgent::setLogFile(QString file)
{
   _logFile = file;
}
void SAgent::log(QString s1, QString s2, QString s3, QString s4)
{
   _lockLog.lock();
   _log.ts(s1 + s2 + s3 + s4);
   _lockLog.unlock();
}
void SAgent::setIP(QString ip)
{
   _IP = ip;
}
QString SAgent::getIP()
{
   return _IP;
}
void SAgent::setPort(int p)
{
   _port = p;
}
int SAgent::port()
{
   return _port;
}
//convert:
 
int SAgent::size()
{
     return packMng.size();
}
bool SAgent::isPack(QString packid)
{
    return packMng.contains(packid);
}
bool SAgent::isApp(QString packid,QString app)
{
    bool b;
    int i;
    b = false;
    if(packMng.contains(packid))
    {
        i = packMng[packid]->find(app);
        if (i >=0)  b= true;
    }
    return b;
}
// ====startUp shutdown();==================
int SAgent::startUp()
{
   //init(); //log,configName;
   QString str;
   int i;
   _config.loadDB();
   i = loadDB();
   return i;
}

int SAgent::down()
{
   qDebug() << "down the agent";
   saveDB();
   _config.saveDB();
   _log.close();
   return 1;

}
