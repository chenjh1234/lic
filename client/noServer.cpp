#include  "noServer.hpp"
void NoServer::init()
{
   LEncrypt en;
    
   _ip = en.getIP().c_str();
   qDebug() <<"ip = " << _ip;
   _user =  en.getUser().c_str();
   _hostname =  en.getHostname().c_str();
   _pid =  en.getPID().c_str();
   _licproject = getenv(ENV_LIC_PROJECT);
   if (_licproject.isEmpty())  _licproject = "null";
   _licAgent = "";
   _licAgent = getenv(ENV_LIC_AGENT);
   _devServer = NULL; 
   hbTH = NULL;
   setPortal(0);
   if (isAgent())  
       setDev(_licAgent);
   else      
       setDev(QString(LIC_SERVER_ID));

}
NoServer::~NoServer()
{
    if (isPortal())  endHB();

}

void NoServer::startHB()
{
   if (hbTH ==NULL)  
        hbTH = new HbThread();
   hbTH->setClient(this);
   hbTH->start(); // if no login ,no connect to server
}
void NoServer::endHB()
{
    if (hbTH !=NULL)  
    {
    
        hbTH->setDown(); 
        hbTH->wait();
        delete hbTH;
     }
}

void NoServer::setDev(QString id)
{
     string str;
     str = id.Q2CH;
     if (_devServer != NULL)  delete _devServer;
     _devServer = new  tetris::device::id(str);
      
}
void NoServer::setPortal(int idx,QString ip,QString port)
{
     _portal = idx;
     _portalIp = ip;
     _portalPort = port;
    if (isPortal())  startHB();
    
}
bool  NoServer::isPortal()
{
    return _portal;
      
}
bool NoServer::isAgent()
{
    return !_licAgent.isEmpty();
}
bool NoServer::startup(helpers *hp)
{
   if (!tetris::device::startup(hp)) return false;
   the_caller = new boost::thread(boost::bind(&NoServer::caller, this));
   //the_hber = new boost::thread(boost::bind(&NoServer::hber, this));
   startHB();
   cout << "client startup ===========================" << endl;
   return the_caller != NULL;
}
void NoServer::shutdown(void)
{
   should_exit = true;
   if (the_caller)
   {
      the_caller->join();
      delete the_caller;
   }
   endHB();

   cout << "client shutdown ===========================" << endl;
}


void NoServer::caller(void)
{
   /* we can omit the last parameter. it defaults to sn_any */

   Json::Value params, result, rm_result;
   Json::Value devs;
   struct timeval t1, t2;
   int i;
   boost::posix_time::time_duration test1	= boost::posix_time::time_duration(0, 0, 3);
   gettimeofday(&t1, NULL);

   cout << "================test call server.method===============" << endl;
#if 0

   const device::id& device,
   const unsigned& function,
   const string& method,
   const Json::Value& params,
   const string& real_token,
   const boost::posix_time::time_duration& timeout,
   Json::Value& result
#endif

   hp->call(the_server, 0, "hello", params, string(), CMD_TIMEOUT, result);
   cout << "hp->call hello  result=" << result.toStyledString() << endl;
   count++;
#if 0
   hp->notify(the_server, 0, "hello", params, string(), tetris::second_5);
   cout << "hp->notify  returned=" << endl;
   // async_call
   hp->async_call(the_server, 0, "hello", params, string(), tetris::second_5, MAKE_DEVICE_ASYNC_CALLBACK(NoServer, async_callback));

   i = hp->get_devices(-1, devs);
   cout << "get devises =" <<  i << devs["list"];
   hp->send(the_server, 0, "hello", -1);

   cout << "================call process\n";
#endif
   if (true) sleep(1);

   gettimeofday(&t2, NULL);
   double delta = t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) / 1000000.f;
   printf("%ld calls made, cps: %0.1f\n", count, count / delta);

}
//====================================
IMPLEMENT_DEVICE_ASYNC_CALLBACK(NoServer, async_callback)
{
   if (finished == true)
   {

      /* async callbacks also need to deal with locking */
      boost::mutex::scoped_lock lock(count_mutex);
      cout << "async call hello ----callback result=" << result["response"] << endl;
      count++;
   }
   else cout << "async call hello ----===failed -----------------" << endl;
}
#define DBUG qDebug()
//===========================================================
void NoServer::shutDown()
{
   QString licStr;
   licStr = "";
   Json::Value params, result;
   JsonParser jp, jr;
   int ir;
   ir = -1;
   jp.set(CMD_PARAM, licStr);
   jp.set(CALL_NOTIFY,"yes");
      if (isAgent()) 
   {
        jp.set(APP_AGENT, _licAgent);
   }
   params = jp.getJ();
  
   qDebug() << "shutDown parms = " << params[CMD_PARAM].asString().c_str();
// call;
    HTTP_CLIENT_START(CMD_SHUTDOWN)
   hp->notify(the_server, 0, CMD_SHUTDOWN, params, string(), CMD_TIMEOUT);
}

int NoServer::loadConfig(QString filename)
{
   LFileDate fd;
   QString licStr;
   licStr = fd.getText(filename);
   Json::Value params, result;
   JsonParser jp, jr;
   int ir;
   ir = -1;

// params:
   if (licStr == "")
   {
      cout << "file  Error,filename = " << filename.Q2CH << "\n";
      return -1;
   }
   jp.set(CMD_PARAM, licStr);
   if (isAgent()) 
   {
        jp.set(APP_AGENT, _licAgent);
   }
   params = jp.getJ();
   //qDebug() << "licStr = " << licStr;
   qDebug() << "load config parms = " << params[CMD_PARAM].asString().c_str();
// call;
   HTTP_CLIENT_START(CMD_LOADCONFIG)
       hp->call(the_server, 0, CMD_LOADCONFIG, params, string(), CMD_TIMEOUT, result); 
   //cout << "hp->call loadfile  result=" << result.toStyledString() << endl;
  
            // HTTP_CLIENT_CLOSE

   jr.setJ(result);
// result:
   qDebug() << "loadConfig ret status = "  << result[RET_STATUS].asString().c_str();
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {
      //cout << " load file ok\n";
      ir = 1;
   }
   //else
   //    cout << " load file error\n";
   return ir;

}
int NoServer::clearConfig(QString passwd)
{
   LFileDate fd;
   Json::Value params, result;
   JsonParser jp, jr;
   int ir;
   ir = -1;

// params:
   if (passwd == "")
   {
      cout << "need a passwd \n";
      return -1;
   }
   jp.set(CMD_PARAM, passwd);
   if (isAgent()) 
   {
        jp.set(APP_AGENT, _licAgent);
   }

   params = jp.getJ();
   //qDebug() << "licStr = " << licStr;
   qDebug() << "load config parms = " << params[CMD_PARAM].asString().c_str();
// call;
    HTTP_CLIENT_START(CMD_CLEARCONFIG)
   hp->call(the_server, 0, CMD_CLEARCONFIG, params, string(),CMD_TIMEOUT, result);
   //cout << "hp->call loadfile  result=" << result.toStyledString() << endl;
   jr.setJ(result);
// result:
   qDebug() << "clearConfig ret status = "  << result[RET_STATUS].asString().c_str();
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {
      ir = 1;
   }
   else
   {
       cout <<  result[RET_MESSAGE].asString().c_str() << endl;
   }
   return ir;
}
QStringList NoServer::viewConfig(QString pack)
{
   Json::Value params, result;
   JsonParser jp, jr;
  QString ret;
  QStringList slist;
// params:

   jp.set(CMD_PARAM, pack);
   if (isAgent())  
       jp.set(APP_AGENT, _licAgent); 
   params = jp.getJ();
   //qDebug() << "licStr = " << licStr;
   qDebug() << "view  config parms = " << params[CMD_PARAM].asString().c_str();
// call;
    HTTP_CLIENT_START(CMD_VIEWCONFIG)
   hp->call(the_server, 0, CMD_VIEWCONFIG, params, string(),CMD_TIMEOUT, result);
   //cout << "hp->call loadfile  result=" << result.toStyledString() << endl;
   jr.setJ(result);
   qDebug() << "viewConfig status = "  << result[RET_STATUS].asString().c_str();
   // result:
  if (jr.get(RET_STATUS) == QString(RET_OK))
  {   
      ret = result[RET_MESSAGE].asString().c_str();
      slist = ret.split("\n");
  }
  else
  {
      slist << "Error : get Config  error!!\n";
  }

   return slist;

}
QStringList NoServer::viewAgent(QString pack)
{
   Json::Value params, result;
   JsonParser jp, jr;
  QString ret;
  QStringList slist;
// params:

   jp.set(CMD_PARAM, pack);

   //qDebug() << "licStr = " << licStr;
   qDebug() << "view  agent parms = " << params[CMD_PARAM].asString().c_str();
   if (!isPortal() || !isAgent() ) 
   {
       slist << "Error : we can only use viewagent with portal & connect to licAgent!!";
       return slist;
   }

    jp.set(APP_AGENT, _licAgent);
    params = jp.getJ();
// call;
    HTTP_CLIENT_START(CMD_VIEWAGENT)
   hp->call(the_server, 0, CMD_VIEWAGENT, params, string(),CMD_TIMEOUT, result);
   //cout << "hp->call loadfile  result=" << result.toStyledString() << endl;
   jr.setJ(result);
    qDebug() << "viewAgent status = "  << result[RET_STATUS].asString().c_str();
    // result:
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {   
       ret = result[RET_MESSAGE].asString().c_str();
       slist = ret.split("\n");
   }
   else
   {
       slist << "Error : get Agent error!!\n";
   }
  

   return slist;

}
int NoServer::loadFile(QString filename)
{
   LFileDate fd;
   QString licStr;
   licStr = fd.getText(filename);
   Json::Value params, result;
   JsonParser jp, jr;
   int ir;
   ir = -1;

// params:
   if (licStr == "")
   {
      cout << "file  Error,filename = " << filename.Q2CH << "\n";
      return -1;
   }
   jp.set(CMD_PARAM, licStr);
   params = jp.getJ();
   //qDebug() << "licStr = " << licStr;
   qDebug() << "load file parms = " << params[CMD_PARAM].asString().c_str();
// call;
    HTTP_CLIENT_START(CMD_LOADFILE)
   hp->call(the_server, 0, CMD_LOADFILE, params, string(),CMD_TIMEOUT, result);
   //cout << "hp->call loadfile  result=" << result.toStyledString() << endl;
   jr.setJ(result);
// result:
   qDebug() << "loadfile ret status = "  << result[RET_STATUS].asString().c_str();
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {
      //cout << " load file ok\n";
      ir = 1;
   }
   //else
   //    cout << " load file error\n";
   return ir;

}
int NoServer::unloadFile(QString filename, QString proofFile)
{
   LFileDate fd;
   QString licStr, proof;
   licStr = fd.getText(filename);
   Json::Value params, result;
   JsonParser jp, jr;
   int ir;
   ir = -1;

// params:
   if (licStr == "")
   {
      cout << "unloadfile file  Error,filename = " << filename.Q2CH << "\n";
      return -1;
   }
   jp.set(CMD_PARAM, licStr);
   params = jp.getJ();
   //qDebug() << "licStr = " << licStr;
   qDebug() << "unload file parms = " << params[CMD_PARAM].asString().c_str();
// call;
    HTTP_CLIENT_START(CMD_UNLOADFILE)
   hp->call(the_server, 0, CMD_UNLOADFILE, params, string(), CMD_TIMEOUT, result);
   //cout << "hp->call loadfile  result=" << result.toStyledString() << endl;
   jr.setJ(result);
// result:
   qDebug() << "unloadfile ret status = "  << result[RET_STATUS].asString().c_str();
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {
      proof = jr.get(RET_MESSAGE);
      fd.writeText(proofFile, proof);
#if 0
      //qDebug() << "proof size = " << proof.split("\n").size();
      if(fd.writeText(proofFile,proof) > 0)
      cout << " unload file ok\n";
      else
      cout << " unload file ok,save proof error\n";
#endif
      cout << proof.Q2CH << endl;

      ir = proof.split("\n").size();
   }
   //else
   // cout << " unload file error\n";
   return ir;
}
QString NoServer::unloadPackage(QString vender,QString pack,QString version,int number,QString ty,QString uuid)
{

   Json::Value params, result;
   JsonParser jp, jr;
   QString str;

   jp.set(APP_VENDER, vender);
   jp.set(APP_PACKAGE, pack);
   jp.set(APP_VERSION, version);
   if (number == 0)  str="";
   else
       str = QString("%1").arg(number);
   jp.set(APP_NUMBER, str); 
   jp.set(APP_NAME, uuid);//for uuid
   jp.set(APP_IP, ty);//for type
    

   params = jp.getJ();
   qDebug() << " unloadPackage  param=" << params.toStyledString().c_str();

   HTTP_CLIENT_START(CMD_UNLOADPACKAGE)
   hp->call(the_server, 0, CMD_UNLOADPACKAGE, params, string(), CMD_TIMEOUT, result);
   jr.setJ(result);

   qDebug() << " unloadPackage result=" << result.toStyledString().c_str();

   str = "";
   
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {
      str = jr.get(RET_MESSAGE);
   }
   //else
   //    cout << " loginApp error\n";

   return str;

}
QString NoServer::removePackage(QString vender,QString pack,QString version,int number,QString ty,QString uuid,QString pass)
{
   Json::Value params, result;
   JsonParser jp, jr;
   QString str;

   jp.set(APP_VENDER, vender);
   jp.set(APP_PACKAGE, pack);
   jp.set(APP_VERSION, version);
   if (number == 0)  str="";
   else
       str = QString("%1").arg(number);

   jp.set(APP_NUMBER, str); 
   jp.set(APP_NAME, uuid);//for uuid
   jp.set(APP_IP, ty);//for type
   jp.set(APP_USER, pass);//for passwd
#if 0
    vender = jp.get(APP_VENDER);
    pack = jp.get(APP_PACKAGE);
    version = jp.get(APP_VERSION);
    number = jp.get(APP_NUMBER);
    ty = jp.get(APP_IP);
    uuid = jp.get(APP_NAME);
    pass = jp.get(APP_USER);
 #endif
   params = jp.getJ();
   qDebug() << " remove package  param=" << params.toStyledString().c_str();

   HTTP_CLIENT_START(CMD_REMOVEPACKAGE)
   hp->call(the_server, 0, CMD_REMOVEPACKAGE, params, string(), CMD_TIMEOUT, result);
   jr.setJ(result);

   qDebug() << " remove package result=" << result.toStyledString().c_str();

   str = "";
   
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {
      str = jr.get(RET_MESSAGE);
   }
   //else
   //    cout << " loginApp error\n";

   return str;

}
QStringList NoServer::loginApp(QString vender, QString packName, QString version, int number, QString appname)
{
   LFileDate fd;
   LEncrypt en;
   QString ip, user, hostname, pid;
   QStringList slist;

   ip = _ip;
   user =  _user;
   hostname =  _hostname;
   pid =  _pid;
   if (appname == "")  appname = "null";


   if (vender.length() <= 0 || packName.length() <= 0)  return slist;

   Json::Value params, result;
   JsonParser jp, jr;

   jp.set(APP_VENDER, vender);
   jp.set(APP_PACKAGE, packName);
   jp.set(APP_VERSION, version);
   jp.set(APP_NUMBER, QString("%1").arg(number));
   jp.set(APP_IP, ip);
   jp.set(APP_HOST, hostname);
   jp.set(APP_NAME, appname);
   jp.set(APP_USER, user);
   jp.set(APP_PID, pid);

   jp.set(APP_LICPROJECT, _licproject);
   if (isAgent()) 
   {
        jp.set(APP_AGENT, _licAgent);
   }
   params = jp.getJ();
   qDebug() << " login app param=" << params.toStyledString().c_str();

   HTTP_CLIENT_START(CMD_LOGINAPP)
   hp->call(the_server, 0, CMD_LOGINAPP, params, string(), CMD_TIMEOUT, result);
   jr.setJ(result);

   qDebug() << " login app result=" << result.toStyledString().c_str();


   QString str;
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {
      str = jr.get(RET_MESSAGE);
      slist = str.split("\n");
      startHB(vender, packName, version);
#if 0
      qDebug() << "slist size = " << str.split("\n").size();
      if(slist.size() >=2)
      cout << " loginApp ok\n";
      else
      cout << " loginApp error\n";
#endif
   }
   //else
   //    cout << " loginApp error\n";

   return slist;
}
int NoServer::logoutApp()
{
   QList<QString> keys;
   QStringList slist;
   
   if (mapHB.size() != 1)  return -1;

   keys = mapHB.keys();
   slist = decodeHB(keys[0]);
   if (slist.size() >=3)  
       return logoutApp(slist[0], slist[1], slist[2]); 
   else if (slist.size() >=2)  
           return logoutApp(slist[0], slist[1],""); 
   else return -1;
}
int NoServer::logoutApp(QString vender, QString packName, QString version)
{

   int ir;
   LEncrypt en;
   QString ip, user, hostname, pid;

   ip = _ip;
   pid = _pid;
   ir = -1;

   Json::Value params, result;
   JsonParser jp, jr;

   jp.set(APP_VENDER, vender);
   jp.set(APP_PACKAGE, packName);
   jp.set(APP_VERSION, version);
   // jp.set(APP_NUMBER,QString("%1").arg(number));
   jp.set(APP_IP, ip);
   //jp.set(APP_HOST,hostname);
   // jp.set(APP_NAME,appname);
   //jp.set(APP_USER,user);
   jp.set(APP_PID, pid); 
   jp.set(APP_LICPROJECT, _licproject);

   if (isAgent()) 
   {
        jp.set(APP_AGENT, _licAgent);
   }

   params = jp.getJ();
   

   qDebug() << " logout app param=" << params.toStyledString().c_str();

    HTTP_CLIENT_START(CMD_LOGOUTAPP)
   hp->call(the_server, 0, CMD_LOGOUTAPP, params, string(), CMD_TIMEOUT, result);
   jr.setJ(result);

   qDebug() << " logout app result=" << result.toStyledString().c_str();

   QStringList slist;
   QString str;
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {
      //cout << " logoutApp ok\n";
      ir = 1;
   }
   //else
   // cout << " logoutApp error\n";
   stopHB(vender, packName, version);
   return ir;
}

void NoServer::heartBeat()
{
   QString v, p, ver, str;
   QStringList slist;
   int sz,i;
   QList<QString> keys;

   keys = mapHB.keys();

   sz = keys.size();
   if (sz == 0) return;

   for (i = 0; i < sz; i++)
   {
      str = keys[i];
      slist = decodeHB(str);
      if (slist.size() >= 3)
      {
         v = slist[0];
         p = slist[1];
         ver = slist[2];
         heartBeat(v, p, ver);
      }
      else if (slist.size() >= 2)
      {
         v = slist[0];
         p = slist[1];
         ver = "";
         heartBeat(v, p, ver);
      }
   }
}
void NoServer::heartBeat(QString vender, QString packName, QString version)
{

   int ir;
   LEncrypt en;
   QString ip, user, hostname, pid;

   ip = _ip;
   pid =  _pid;
   ir = -1;

   Json::Value params, result;
   JsonParser jp, jr;

   jp.set(APP_VENDER, vender);
   jp.set(APP_PACKAGE, packName);
   jp.set(APP_VERSION, version);
   // jp.set(APP_NUMBER,QString("%1").arg(number));
   jp.set(APP_IP, ip);
   //jp.set(APP_HOST,hostname);
   // jp.set(APP_NAME,appname);
   jp.set(CALL_NOTIFY,"yes");
   jp.set(APP_PID, pid);
    if (isAgent()) 
   {
        jp.set(APP_AGENT, _licAgent);
   }
   params = jp.getJ();

   qDebug() << " heartBeat app param=" << params.toStyledString().c_str();
    HTTP_CLIENT_START(CMD_HEARTBEAT)
   hp->notify(the_server, 0, CMD_HEARTBEAT, params, string(), CMD_TIMEOUT);

}
int NoServer::borrow(QString filein, QString fileout)
{
   LFileDate fd;
   QString licStr;
   licStr = fd.getText(filein);

   Json::Value params, result;
   JsonParser jp, jr;
   int ir;
   ir = -1;

// params:
   if (licStr == "")
   {
      cout << "file  Error,filename = " << filein.Q2CH << "\n";
      return -1;
   }
   jp.set(CMD_PARAM, licStr);
   params = jp.getJ();
   //qDebug() << "licStr = " << licStr;
   qDebug() << "borrow parms = " << params[CMD_PARAM].asString().c_str();
// call;
    HTTP_CLIENT_START(CMD_BORROW)
   hp->call(the_server, 0, CMD_BORROW, params, string(), CMD_TIMEOUT, result);
   //cout << "hp->call loadfile  result=" << result.toStyledString() << endl;
   jr.setJ(result);
// result:
   qDebug() << "borrow ret status = " << result[RET_STATUS].asString().c_str();
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {

      licStr = jr.get(RET_MESSAGE);
      if (licStr.length() > 0)
      {
         if (fd.writeText(fileout, licStr) <= 0) fd.writeText("/tmp/borrow_tmp.lic", licStr);
         ir = 1;
         cout << licStr.Q2CH << "\n";
      }
   }
   return ir;
}
int NoServer::borrowReturn(QString proofFile)
{
   LFileDate fd;
   QString proof;
   proof = fd.getText(proofFile);

   Json::Value params, result;
   JsonParser jp, jr;
   int ir;
   ir = -1;

// params:
   if (proof == "")
   {
      cout << "proofFile  Error,filename = " << proofFile.Q2CH << "\n";
      return -1;
   }
   jp.set(CMD_PARAM, proof);
   params = jp.getJ();
   //qDebug() << "licStr = " << licStr;
   qDebug() << "borrowReturn parms = " << params[CMD_PARAM].asString().c_str();
// call;
    HTTP_CLIENT_START(CMD_BORROWRETURN)
   hp->call(the_server, 0, CMD_BORROWRETURN, params, string(), CMD_TIMEOUT, result);
   //cout << "hp->call loadfile  result=" << result.toStyledString() << endl;
   jr.setJ(result);
// result:
   qDebug() << "borrowReturn ret status = " << result[RET_STATUS].asString().c_str();
   if (jr.get(RET_STATUS) == QString(RET_OK))    ir = 1;
   return ir;
}
QStringList NoServer::report(QString param, QString param1)
{
   QStringList slist;
   LFileDate fd;

   Json::Value params, result;
   JsonParser jp, jr;

   int ir;
   ir = -1;
// params:
   if (!(param == REPORT_PARAM_PACKAGE || param == REPORT_PARAM_ALL || param == REPORT_PARAM_APP)) param =  REPORT_PARAM_PACKAGE;

   jp.set(REPORT_PARAM, param);
   if (param1 != QString(""))  jp.set(REPORT_PARAM_SINGLE, param1);

   params = jp.getJ();
   //qDebug() << "licStr = " << licStr;
   qDebug() << "report parms = " << params[REPORT_PARAM].asString().c_str() << params[REPORT_PARAM_SINGLE].asString().c_str() ;
// call;
    HTTP_CLIENT_START(CMD_REPORT)
   hp->call(the_server, 0, CMD_REPORT, params, string(), CMD_TIMEOUT, result);
  
   //cout << "hp->call report  result=" << result.toStyledString() << endl;
   jr.setJ(result);
// result:
   qDebug() << "report ret status = " << jr.get(RET_STATUS);
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {
      //cout << " report ok\n";
      ir = 1;
   }
   //else
   //    cout << " report error\n";
   QString str;
   str = jr.get(RET_MESSAGE);
   if (str.length() > 0)  slist = str.split("\n");
   return slist;
}
#if 0
bool NoServer::setCheckUUID(bool b)
{

   QString para;
   if (b) para = "1";
   else para = "0";

   Json::Value params, result;
   JsonParser jp, jr;
   bool rb;
   rb = false;

// params:

   jp.set(CMD_PARAM, para);
   params = jp.getJ();

   qDebug() << "checkUUID parms = " << params[CMD_PARAM].asString().c_str();
// call;
   hp->call(the_server, 0, CMD_CHECKUUID, params, string(), tetris::second_10, result);
   //cout << "hp->call loadfile  result=" << result.toStyledString() << endl;
   jr.setJ(result);
// result:
   qDebug() << "checkUUID ret status = " << result[RET_STATUS].asString().c_str();
   if (jr.get(RET_STATUS) == QString(RET_OK))
   {

      para = jr.get(RET_MESSAGE);
      if (b && para == "1")  rb = true;
      if (!b && para == "0") rb = true;
   }
   return rb;
}

void NoServer::hber()
{
   int sz, i;
   while (!should_exit)
   {
      heartBeat();
      //
      qDebug() << "===HB+++++++++++++++++++++";
      sleep(APP_HB_INT);
   }
}
#endif
QString NoServer::encodeHB(QString vender, QString pack, QString version)
{
   QString str;
   str = vender + "\n" + pack + "\n" + version;
   return str;

}
QStringList NoServer::decodeHB(QString enHB)
{
   QStringList slist;
   slist = enHB.split("\n");
   return slist;
}
void NoServer::startHB(QString vender, QString pack, QString version)
{
   QString str;
   str = encodeHB(vender, pack, version);
   mapHB[str] = str;
}
void NoServer::stopHB(QString vender, QString pack, QString version)
{
   QString str;
   str = encodeHB(vender, pack, version);
   mapHB.remove(str);
}
