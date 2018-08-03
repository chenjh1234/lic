#include  "licAgent.hpp"
LicAgent::LicAgent (): count(0) 
{
    _devServer = NULL;
    setDev(QString(LIC_SERVER_ID));
}
unsigned LicAgent::get_functions(void)
{
   return CMD_AGENT;
}
unsigned LicAgent::get_methods(const unsigned& function, method_map& methods)
{
   /* we have only one method, too */
   if (function == 0)
   {
      ADD_DEVICE_METHOD(methods, LicAgent, hello);


      ADD_DEVICE_METHOD(methods, LicAgent, loginApp);
      ADD_DEVICE_METHOD(methods, LicAgent, logoutApp);
      ADD_DEVICE_METHOD(methods, LicAgent, heartBeat);
      ADD_DEVICE_METHOD(methods, LicAgent, shutDown);
      ADD_DEVICE_METHOD(methods, LicAgent, loadConfig);
      ADD_DEVICE_METHOD(methods, LicAgent, viewConfig);
      ADD_DEVICE_METHOD(methods, LicAgent, clearConfig);
      ADD_DEVICE_METHOD(methods, LicAgent, viewAgent);
      //ADD_DEVICE_METHOD(methods, LicAgent, loadFile);

   }
   return methods.size();
}
 
bool LicAgent::startup(helpers *hp)
{
     QString str;
     _startupFailed = false;
   /* startup the base and remember the time */
   if (tetris::device::startup(hp))
   {
       
       _down = 0;
      gettimeofday(&startup_time, NULL);
      cout << "start up LicAgent------- ";
//--------------------------------------------
   
      if (   mng.startUp() < 0) 
      {
          cout << "start up LicAgent------- failed\n";
          str = "start up LicAgent------- failed";
          mng.log(str);
          _startupFailed = true;
          return false;
      }
     
      str = "start up LicAgent-------OK ";
      cout << "start up LicAgent-------OK\n";
      mng.log(str);
      the_cyc = new boost::thread(boost::bind(&LicAgent::thCyc, this));
      // start circle;
      //cyc.setRole(ROLE_AGENT);
      //cyc.start();
      return the_cyc;
   }
   return false;
}
void LicAgent::shutdown(void)
{
   /* tell people how fast we can go */
   gettimeofday(&shutdown_time, NULL);
   double delta = shutdown_time.tv_sec - startup_time.tv_sec +
      (shutdown_time.tv_usec - startup_time.tv_usec) / 1000000.f;
   printf("%ld requests serviced, rps: %0.1f\n", count, count / delta);
   //--------------------------------
   if (! _startupFailed )  
   {
       QString str;
     
       _down = 1;

       if (the_cyc)
      {
           the_cyc->join();
           delete the_cyc;
      }

       str = "shutdown  LicAgent-------OK ";
       mng.log(str);
    // save db,close log:
       mng.down();
       
     }

}
#if 1
//==============================================================Methed:=================
IMPLEMENT_DEVICE_METHOD(LicAgent, hello)
{
   /* device methods are called by the bus cocurrently, so
    * they have to deal with locking while accessing shared variables */
   boost::mutex::scoped_lock lock(count_mutex);
   result["response"] = Json::Value(" cpu xxxxx % %%%%%%%%%%");
   // printf("accept method= %s\n", method.c_str());
   count++;
   /*test get_devices*/
   //tetris::device::id fliter(-2,-2,-2,-2);//self
   tetris::device::id fliter(-1, -1, -1, -1);
   Json::Value devices;
   return tetris::device::method_finished;

}
#endif
IMPLEMENT_DEVICE_PROCESSOR(LicAgent, pro)
{
   /* device methods are called by the bus cocurrently, so
* they have to deal with locking while accessing shared variables */
   cout << "msg ======" << message << endl;
   cout << "-----------in process server\n";

}
//=interface for lice ==========================================
IMPLEMENT_DEVICE_METHOD(LicAgent,loadConfig)
{
    int ir;
    QString licStr;
    JsonParser jp(params),jr;
    qDebug() << "===========================in server LoadConfig parms = " << params[CMD_PARAM].asString().c_str();
//in
    licStr = jp.get(CMD_PARAM);
    //qDebug() << "in server call before" ;
//do
    ir = mng._config.loadStr(licStr);
     //qDebug() << "in server call after" ;

    if (ir <0)  jr.set(RET_STATUS,RET_ERR);
    else
        jr.set(RET_STATUS,RET_OK);
//out:
    result = jr.getJ();
    qDebug() << "load config in server result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
     
}

//=interface for lice ==========================================
IMPLEMENT_DEVICE_METHOD(LicAgent,clearConfig)
{
    int ir;
    QString licStr;
    JsonParser jp(params),jr;
    qDebug() << "===========================in server clearConfig= " << params[CMD_PARAM].asString().c_str();
//in
    licStr = jp.get(CMD_PARAM);
    //passwd
//do
    //ir = mng.clearConfig(licStr);
     //qDebug() << "in server call after" ;
    LFileDate fd;
    int iret,i;
    if (fd.isPasswdToday(licStr,"clearconfig"))  
    {
            i = mng._config.clear();
            if (i >=0)  iret = 0;
            else iret = -2;
    }
    else 
        iret = -1;
    ir =  iret;
    if (ir <0) 
    {
         jr.set(RET_STATUS,RET_ERR);
         if( ir == -1) jr.set(RET_MESSAGE,"passwd error");
         if( ir == -2) jr.set(RET_MESSAGE,"clearconfig error");  
    }
    else
        jr.set(RET_STATUS,RET_OK);
  
//out:
    result = jr.getJ();
    qDebug() << "clear config in agent result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
     
}
//=interface for lice ==========================================
IMPLEMENT_DEVICE_METHOD(LicAgent,viewConfig)
{
    int ir;
    QString pack,ret;
    JsonParser jp(params),jr;
    qDebug() << "==============in agent viewConfig parms = " << params[CMD_PARAM].asString().c_str();
//in
    pack = jp.get(CMD_PARAM);
    ret = mng._config.view(pack);
     //qDebug() << "in server call after" ;
    jr.set(RET_STATUS,RET_OK);
    jr.set(RET_MESSAGE,ret);
//out:
    result = jr.getJ();
    qDebug() << "view config in agent result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
     
}
 //=interface for lice ==========================================
IMPLEMENT_DEVICE_METHOD(LicAgent,viewAgent)
{
    int ir;
    QString pack,ret;
    JsonParser jp(params),jr;
    qDebug() << "==============in agent viewAgent parms = " << params[CMD_PARAM].asString().c_str();
//in
    pack = jp.get(CMD_PARAM);
    ret = mng.view(pack);
     //qDebug() << "in server call after" ;
    jr.set(RET_STATUS,RET_OK);
    jr.set(RET_MESSAGE,ret);
//out:
    result = jr.getJ();
    qDebug() << "view agent in agent result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
     
}
 
IMPLEMENT_DEVICE_METHOD(LicAgent,loginApp)
{
    
    int ir;
    SAppInfo info;
    JsonParser jp(params),jr;
    qDebug() << "=================in agent loginApp parms = " << params[APP_PID].asString().c_str();
//in
    info.set(APP_VENDER,jp.get(APP_VENDER));
    info.set(APP_PACKAGE,jp.get(APP_PACKAGE));
    info.set(APP_VERSION,jp.get(APP_VERSION));
    info.set(APP_NUMBER,jp.get(APP_NUMBER));
    info.set(APP_IP,jp.get(APP_IP));
    info.set(APP_HOST,jp.get(APP_HOST));
    info.set(APP_NAME,jp.get(APP_NAME));
    info.set(APP_USER,jp.get(APP_USER));
    info.set(APP_PID,jp.get(APP_PID));
    info.set(APP_AGENT,jp.get(APP_AGENT));
    //qDebug() << "loginApp agent = " << jp.get(APP_AGENT);

    info.set(APP_LICPROJECT,jp.get(APP_LICPROJECT));
     
//do
    QStringList slist;
    slist =  loginApp(info);
    
    QString str;
    if (slist.size()  <2 )  jr.set(RET_STATUS,RET_ERR);
    else
    {
        jr.set(RET_STATUS,RET_OK);
        str = slist[0];
        str += "\n";
        str += slist[1];
        jr.set(RET_MESSAGE,str);
    }
//out:
    result = jr.getJ();
    qDebug() << "loginApp in agent result = " << result[RET_STATUS].asString().c_str();

    return tetris::device::method_finished;
}
IMPLEMENT_DEVICE_METHOD(LicAgent,logoutApp)
{

    int ir;
    SAppInfo info;
    JsonParser jp(params),jr;
    qDebug() << "==================in agent logOut parms = " << params[APP_PID].asString().c_str();
//in
    info.set(APP_VENDER,jp.get(APP_VENDER));
    info.set(APP_PACKAGE,jp.get(APP_PACKAGE));
    info.set(APP_VERSION,jp.get(APP_VERSION));
    //info.set(APP_NUMBER,jp.get(APP_NUMBER));
    info.set(APP_IP,jp.get(APP_IP));
    //info.set(APP_HOST,jp.get(APP_HOST));
    //info.set(APP_NAME,jp.get(APP_NAME));
    //info.set(APP_USER,jp.get(APP_USER));
    info.set(APP_PID,jp.get(APP_PID));
    info.set(APP_LICPROJECT,jp.get(APP_LICPROJECT));
     qDebug() << "  parms = "<<jp.get(APP_VENDER) <<jp.get(APP_PACKAGE) <<jp.get(APP_VERSION) << jp.get(APP_IP) << jp.get(APP_PID);

    ir = unloginApp(info);
    qDebug() << "logout end ="<< ir;
    
    QString str;
    if (ir <0)  jr.set(RET_STATUS,RET_ERR);
    else
    {
        jr.set(RET_STATUS,RET_OK);
    }
//out:
    result = jr.getJ();
    qDebug() << "logoutApp in agent  result = " << result[RET_STATUS].asString().c_str();

    return tetris::device::method_finished;
}
IMPLEMENT_DEVICE_METHOD(LicAgent,heartBeat)
{
    int ir;
    SAppInfo info;
    JsonParser jp(params),jr;
    qDebug() << "================in server hreartBeat parms = " << params[APP_PID].asString().c_str();
//in
    info.set(APP_VENDER,jp.get(APP_VENDER));
    info.set(APP_PACKAGE,jp.get(APP_PACKAGE));
    info.set(APP_VERSION,jp.get(APP_VERSION));
    //info.set(APP_NUMBER,jp.get(APP_NUMBER));
    info.set(APP_IP,jp.get(APP_IP));
    //info.set(APP_HOST,jp.get(APP_HOST));
    //info.set(APP_NAME,jp.get(APP_NAME));
    //info.set(APP_USER,jp.get(APP_USER));
    info.set(APP_PID,jp.get(APP_PID));
     
//do
    ir = mng.appHB(info);
    
    QString str;
    if (ir <0)  jr.set(RET_STATUS,RET_ERR);
    else
    {
        jr.set(RET_STATUS,RET_OK);
    }
//out:
    result = jr.getJ();
    //qDebug() << "loginApp in server result = " << result[RET_STATUS].asString().c_str();

    return tetris::device::method_finished;
}
 
IMPLEMENT_DEVICE_METHOD(LicAgent,shutDown)
{
    SLicMng mng;

    QString licStr;
    JsonParser jp(params),jr;
    qDebug() << "===========================in server shutDown = " << params[CMD_PARAM].asString().c_str();

    _down = 1;
    qDebug() << "in agent call after _down = " << _down ;
    jr.set(RET_STATUS,RET_OK);
//out:int
    result = jr.getJ();
   //qDebug() << "shutdown in server result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
     
}
QStringList LicAgent::loginApp(SAppInfo &msg)
{
   int i,ii;
   QString  err;
   QStringList slist;
   i = -10;
   if (mng.isLoginConfig(msg))  // if login config, we do login app;
   {
      slist  =  serverLogin(msg);
       if (slist.size() >=2)  i = 1;
   }
   else
   {
      QString  appname = msg.get(APP_NAME).toString();
      // config limited:
      err =  msg.encodePackageID() + " --- " + appname + "_" +  msg.encodeAppID() + "--- config limited error";
      mng.log(err);
      return slist;
   }

   if (i <= 0) // if login config  and : no license space , we unlogin config
   {
      ii = mng.isLogoutConfig(msg);
      if (ii < 0)   mng.log("logout config error ", msg.encodePackageID(), " ", msg.encodeAppID());
   }
   else// serverlogin OK
   {
       // remove APP_AGENT KEY: make the  appInfo checkHB works:
      msg.remove(APP_AGENT);
      i = mng.registerApp(msg);
   }
   return slist;

}
int LicAgent::unloginApp(SAppInfo &msg)
{
       int i;
   QString err;

   i =  serverLogout(msg);
   if (i >= 0)  // logout ok we unlogin config;
   {
      i = mng.isLogoutConfig(msg);
      if (i < 0)   mng.log("logout config error ", msg.encodePackageID(), " ", msg.encodeAppID());
       qDebug() << "size0 = " << mng.size();
      i = mng.unRegisterApp(msg);
      qDebug() << "size1 = " << mng.size() << i;
      
   }
   else
   {
      err = "httpLogout error " + msg.encodePackageID() + " " + msg.encodeAppID();
      mng.log(err);
      i = mng.isLogoutConfig(msg);
      if (i < 0)   mng.log("logout config error ", msg.encodePackageID(), " ", msg.encodeAppID());
      i = mng.unRegisterApp(msg);
 
   }
   return i;

}

QStringList LicAgent::serverLogin(SAppInfo &app)
{
   LFileDate fd;
   LEncrypt en;
   QString ip, user, hostname, pid;
   QString packName,vender;
   QString version,number,appname,licproject;
   QStringList slist;
   Json::Value params, result;
   JsonParser jp, jr;
#if 0
   ip = app.get(APP_IP).toString();
   user = app.get(APP_USER).toString();
   hostname = app.get(APP_HOST).toString();
   pid = app.get(APP_PID).toString();
   appname = app.get(APP_NAME).toString();

   vender = app.get(APP_VENDER).toString();
   packName = app.get(APP_PACKAGE).toString();
   version = app.get(APP_VERSION).toString();
   number = app.get(APP_NUMBER).toString();
   licproject = app.get(APP_LICPROJECT).toString();
   if (vender.length() <= 0 || packName.length() <= 0)  return slist;
   jp.set(APP_VENDER, vender);
   jp.set(APP_PACKAGE, packName);
   jp.set(APP_VERSION, version);
   //jp.set(APP_NUMBER, QString("%1").arg(number));
   jp.set(APP_NUMBER,  number);
   jp.set(APP_IP, ip);
   jp.set(APP_HOST, hostname);
   jp.set(APP_NAME, appname);
   jp.set(APP_USER, user);
   jp.set(APP_PID, pid);
   jp.set(APP_LICPROJECT, _licproject); 
   params = jp.getJ();
#endif

   vender = app.get(APP_VENDER).toString();
   packName = app.get(APP_PACKAGE).toString();
   if (vender.length() <= 0 || packName.length() <= 0)  return slist;
   params  = toJason(app);
   qDebug() << " login app param in agent =" << params.toStyledString().c_str();
   hp->call(the_server, 0, CMD_LOGINAPP, params, string(), CMD_TIMEOUT, result);
   qDebug() << " login app result=" << result.toStyledString().c_str();


   QString str;
   if ( getJsonV(result,RET_STATUS) == QString(RET_OK))
   {
      str = getJsonV(result,RET_MESSAGE);
      slist = str.split("\n");
   }
   return slist;
}

int LicAgent::serverLogout(SAppInfo &app)
{
   int ir;
   LEncrypt en;
   QString ip, user, hostname, pid;
   ir = -1;

   Json::Value params, result;
   JsonParser jp, jr;

   params = toJason(app);
 
   qDebug() << " logout app in agent param=" << params.toStyledString().c_str();
   hp->call(the_server, 0, CMD_LOGOUTAPP, params, string(), CMD_TIMEOUT, result);
   qDebug() << " logout app in agent  result=" << result.toStyledString().c_str();

   QString str;
   if ( getJsonV(result,RET_STATUS) == QString(RET_OK))
   {
       ir = 1;
   }
   return ir;
}

void LicAgent::setDev(QString id)
{
     string str;
     str = id.Q2CH;
     if (_devServer != NULL)  delete _devServer;
     _devServer = new  tetris::device::id(str);
      
}
 
Json::Value   LicAgent::toJason(SAppInfo& info)
{
   Json::Value j;
   string k, v;
   int i, sz;
   QString qk, qv;
   QStringList slist;
   sz = info.size();
   slist = info.nameList();
   for (i = 0; i < sz; i++)
   {
      qk = slist[i];
      qv = info.get(qk).toString();
      k = qk.Q2CH;
      v = qv.Q2CH;
      j[k] = v;
   }
   return j;
}
QString LicAgent::getJsonV(Json::Value& jv, QString key)
{
   QString qv;
   string v, k;
   k = key.Q2CH;
   if (jv.isMember(k)) v = jv[k].asString();
   else v = "";
   qv = v.c_str();
   return qv;
}
bool  LicAgent::isDown()
{
    return _down;
}
int LicAgent::thCyc()
{
    //long dc;//daycheck
    int ic;
    LFileDate lfd;
    long _dbSave =  lfd.sEP(); 

    while (1) 
    {
        ic = 0;
        qDebug() << "HB_INT =" << HEARTBEAT_INT << lfd.sEP()<<lfd.curDT();//<< TEST_CYCLE;
        while (ic < HEARTBEAT_INT) 
        {
            sleep(1); // delay maxmun = 1;
            ic++;
            if (isDown()) break;
        }
        if (isDown())  
        {
            qDebug() <<  "i down----------------------";
            break;
        }
        checkHB(HEARTBEAT_INT);
        //  qDebug() << "HB OK"<< lfd.sEP()<<lfd.curDT();
        // dbsave: 
        if (lfd.sEP() - _dbSave >= DB_SAVE_INT) 
        {
            qDebug() << "DBSAVE start";
            _dbSave = lfd.sEP();
            mng.saveDB();
            qDebug() << "DBSAVE OK"<< lfd.sEP()<<lfd.curDT();
        }
        if (isDown())  
            break;
 
    }
    return 1;
 
}

int LicAgent::checkHB(long intvs)
{
   int i, sz, ir, ic;
   QStringList packs;
   QString pack;
   ic = 0;

   packs = mng.packMng.keys();
   sz = packs.size();
   for (i = 0; i < sz; i++)
   {
      pack = packs[i];
      ir = checkHB(pack, intvs);
      ic = ic + ir;
   }
   return ic;
}
int LicAgent::checkHB(QString pack, long intvs)
{

   // packid,appid,user,number type, start
   int i, sz, ir, ic;
   QStringList  slist;
   SAppMng *appmng;
   SAppInfo *info;
   QString appid, user, ty, str, start;
   //long start;
   LFileDate fd;
   appmng = NULL;
  appmng = mng.packMng[pack];
   //qDebug() << " checkHB0 =" << pack << mng;

   if (appmng == NULL) return 0;

   sz = appmng->size();
   //qDebug() << " checkHB =" << pack << sz;
   ic = 0;

   for (i = 0; i < sz; i++)
   {
      //qDebug() << "checkHB0 =" << i << pack;
      info = (SAppInfo *)appmng->get(sz - i - 1);
      //qDebug() << "checkHB =" << i << pack << info->appid;
      if (info != NULL)  
          if (info->checkHB(intvs))
         {
            ic++;
            appid = info->appid;
            user = info->user;
            ir =unloginApp(*info);
            if (ir > 0)
            {
               qDebug() << "check hearBeat rmApp OK appid = " << pack << appid << user;
               mng.log("check hearBeat rmApp OK appid = ", pack, appid);
            }
            else
            {
               qDebug() << "check hearBeat rmApp Err appid = " << pack << appid << user;
               mng.log("check hearBeat rmApp Err appid = ", pack, appid);
            }
         }
      //qDebug() << "checkHB1 =" << i << pack;
   }

   return ic;
}
