#include  "licServer.hpp"

unsigned LicServer::get_functions(void)
{
   return CMD_NUMBERS;
}
unsigned LicServer::get_methods(const unsigned& function, method_map& methods)
{
   /* we have only one method, too */
   if (function == 0)
   {
      ADD_DEVICE_METHOD(methods, LicServer, hello);

      ADD_DEVICE_METHOD(methods, LicServer, loadFile);
      ADD_DEVICE_METHOD(methods, LicServer, unloadFile);
      ADD_DEVICE_METHOD(methods, LicServer, unloadPackage);
      ADD_DEVICE_METHOD(methods, LicServer, removePackage);
      ADD_DEVICE_METHOD(methods, LicServer, loginApp);
      ADD_DEVICE_METHOD(methods, LicServer, logoutApp);
      ADD_DEVICE_METHOD(methods, LicServer, heartBeat);
      ADD_DEVICE_METHOD(methods, LicServer, borrow);
      ADD_DEVICE_METHOD(methods, LicServer, borrowReturn);
      ADD_DEVICE_METHOD(methods, LicServer, report);
      ADD_DEVICE_METHOD(methods, LicServer, shutDown);
      ADD_DEVICE_METHOD(methods, LicServer, loadConfig);
      ADD_DEVICE_METHOD(methods, LicServer, viewConfig);
      ADD_DEVICE_METHOD(methods, LicServer, clearConfig);
      //ADD_DEVICE_METHOD(methods, LicServer, loadFile);
      //ADD_DEVICE_METHOD(methods, LicServer, loadFile);

   }
   return methods.size();
}
#if 0
//processor_type get_processor (const unsigned& function);
processor_type LicServer::get_processor(const unsigned& function)
{
   /* we have only one method, too */
   // if (function == 0)
   return MAKE_DEVICE_PROCESSOR(LicServer,pro);


}
#endif
bool LicServer::startup(helpers *hp)
{
     QString str;
     _startupFailed = false;
   /* startup the base and remember the time */
   if (tetris::device::startup(hp))
   {
       _down = 0;
      gettimeofday(&startup_time, NULL);
      cout << "start up licserver------- ";
//--------------------------------------------
      SLicMng mng;
      if ( ! mng.startup()) 
      {
          cout << "start up licserver------- failed\n";
          str = "start up licserver------- failed";
          mng.data->plog(str);
          _startupFailed = true;
          return false;
      }
     
      str = "start up licserver-------OK ";
      cout << "start up licserver-------OK\n";
      mng.data->plog(str);
      // start circle;
      cyc.start();
      return true;
   }
   return false;
}
void LicServer::shutdown(void)
{
   /* tell people how fast we can go */
   gettimeofday(&shutdown_time, NULL);
   double delta = shutdown_time.tv_sec - startup_time.tv_sec +
      (shutdown_time.tv_usec - startup_time.tv_usec) / 1000000.f;
   printf("%ld requests serviced, rps: %0.1f\n", count, count / delta);
   //--------------------------------
   if (! _startupFailed )  
   {
       SLicMng mng; 
       QString str;
       str = "shutdown  licserver-------OK ";
       mng.data->plog(str);

    // down the cyc thread:
       cyc.setDown();
       cyc.wait();

    // save db;
       mng.down();
     }

    delete SLicMng::data;
}
#if 1
//==============================================================Methed:=================
IMPLEMENT_DEVICE_METHOD(LicServer, hello)
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
IMPLEMENT_DEVICE_PROCESSOR(LicServer, pro)
{
   /* device methods are called by the bus cocurrently, so
* they have to deal with locking while accessing shared variables */
#if 0
   boost::mutex::scoped_lock lock(count_mutex);
   result["response"] = Json::Value(" cpu xxxxx % %%%%%%%%%%");
   printf("accept method= %s\n", method.c_str());
   count++;
   /*test get_devices*/
   //tetris::device::id fliter(-2,-2,-2,-2);//self
   tetris::device::id fliter(-1, -1, -1, -1);
   Json::Value devices;
   return tetris::device::method_finished;
#endif
   cout << "msg ======" << message << endl;
   cout << "-----------in process server\n";

}
//=interface for lice ==========================================
IMPLEMENT_DEVICE_METHOD(LicServer,loadConfig)
{
    SLicMng mng;
    int ir;
    QString licStr;
    JsonParser jp(params),jr;
    qDebug() << "===========================in server LoadConfig parms = " << params[CMD_PARAM].asString().c_str();
//in
    licStr = jp.get(CMD_PARAM);
    //qDebug() << "in server call before" ;
//do
    ir = mng.data->_config.loadStr(licStr);
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
IMPLEMENT_DEVICE_METHOD(LicServer,clearConfig)
{
    SLicMng mng;
    int ir;
    QString licStr;
    JsonParser jp(params),jr;
    qDebug() << "===========================in server clearConfig= " << params[CMD_PARAM].asString().c_str();
//in
    licStr = jp.get(CMD_PARAM);
    //passwd
//do
    ir = mng.clearConfig(licStr);
     //qDebug() << "in server call after" ;

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
    qDebug() << "clear config in server result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
     
}
//=interface for lice ==========================================
IMPLEMENT_DEVICE_METHOD(LicServer,viewConfig)
{
    SLicMng mng;
    int ir;
    QString pack,ret;
    JsonParser jp(params),jr;
    qDebug() << "==============in server viewConfig parms = " << params[CMD_PARAM].asString().c_str();
//in
    pack = jp.get(CMD_PARAM);
    ret = mng.data->_config.view(pack);
     //qDebug() << "in server call after" ;

    jr.set(RET_STATUS,RET_OK);
    jr.set(RET_MESSAGE,ret);
//out:
    result = jr.getJ();
    qDebug() << "view config in server result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
     
}
IMPLEMENT_DEVICE_METHOD(LicServer,loadFile)
{
    SLicMng mng;
    int ir;
    QString licStr;
    JsonParser jp(params),jr;
    qDebug() << "===========================in server LoadConfigparms = " << params[CMD_PARAM].asString().c_str();
//in
    licStr = jp.get(CMD_PARAM);
    //qDebug() << "in server call before" ;
//do
    ir = mng.loadFileStr(licStr);
     //qDebug() << "in server call after" ;

    if (ir <0)  jr.set(RET_STATUS,RET_ERR);
    else
        jr.set(RET_STATUS,RET_OK);
//out:
    result = jr.getJ();
    qDebug() << "load file in server result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
     
}
IMPLEMENT_DEVICE_METHOD(LicServer,unloadFile)
{
    SLicMng mng;
    int ir;
    QString licStr,proof;
    JsonParser jp(params),jr;
    qDebug() << "==================in server unLoadfile parms = " << params[CMD_PARAM].asString().c_str();
//in
    licStr = jp.get(CMD_PARAM);
    //qDebug() << "in server liStr = " << licStr;
//do
    ir = mng.unloadFileStr(licStr,proof);
    //int unloadFileStr(QString str,QString &proofStr);
    if (ir <0)  jr.set(RET_STATUS,RET_ERR);
    else
    {
        jr.set(RET_STATUS,RET_OK);
        jr.set(RET_MESSAGE,proof);
    }
//out:
    result = jr.getJ();
    qDebug() << "unload file in server result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
}
IMPLEMENT_DEVICE_METHOD(LicServer,unloadPackage)
{
    
    SLicMng mng;
    int ir;
    SAppInfo info;
    JsonParser jp(params),jr;
    QString vender,pack,version,number,ty,uuid,str;
    qDebug() << "=================in server unloadPackage parms = " << params.toStyledString().c_str();
//in
    vender = jp.get(APP_VENDER);
    pack = jp.get(APP_PACKAGE);
    version = jp.get(APP_VERSION);
    number = jp.get(APP_NUMBER);
    ty = jp.get(APP_IP);
    uuid = jp.get(APP_NAME);

//do
    str = mng.unloadPackage(vender,pack,version,number,ty,uuid);
    
    if (str.length() == 0)  jr.set(RET_STATUS,RET_ERR);
    else
    {
        jr.set(RET_STATUS,RET_OK);
        jr.set(RET_MESSAGE,str);
    }
//out:
    result = jr.getJ();
    qDebug() << "unloadPackage in server result = " << result[RET_STATUS].asString().c_str();

    return tetris::device::method_finished;
}
IMPLEMENT_DEVICE_METHOD(LicServer,removePackage)
{
    
    SLicMng mng;
    int ir;
    SAppInfo info;
    JsonParser jp(params),jr;
    QString vender,pack,version,number,ty,uuid,str,pass;
    qDebug() << "=================in server removePackage parms = " << params.toStyledString().c_str();
//in
    vender = jp.get(APP_VENDER);
    pack = jp.get(APP_PACKAGE);
    version = jp.get(APP_VERSION);
    number = jp.get(APP_NUMBER);
    ty = jp.get(APP_IP);
    uuid = jp.get(APP_NAME);
    pass = jp.get(APP_USER);

//do
    str = mng.unloadPackage(vender,pack,version,number,ty,uuid,pass);
    
    if (str.length() == 0)  jr.set(RET_STATUS,RET_ERR);
    else
    {
        jr.set(RET_STATUS,RET_OK);
        jr.set(RET_MESSAGE,str);
    }
//out:
    result = jr.getJ();
    qDebug() << "removePackage in server result = " << result[RET_STATUS].asString().c_str();

    return tetris::device::method_finished;
}
IMPLEMENT_DEVICE_METHOD(LicServer,loginApp)
{
    
    SLicMng mng;
    int ir;
    SAppInfo info;
    JsonParser jp(params),jr;
    qDebug() << "=================in server loginApp parms = " << params[APP_PID].asString().c_str();
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

    info.set(APP_LICPROJECT,jp.get(APP_LICPROJECT));
     
//do
    ir = mng.loginApp(info);
    
    QString str;
    if (ir <0)  jr.set(RET_STATUS,RET_ERR);
    else
    {
        jr.set(RET_STATUS,RET_OK);
        str = info.get(APP_VENDERSIGN).toString();
        str += "\n";
        str += info.get(APP_PBORROW).toString();
        jr.set(RET_MESSAGE,str);
    }
//out:
    result = jr.getJ();
    qDebug() << "loginApp in server result = " << result[RET_STATUS].asString().c_str();

    return tetris::device::method_finished;
}
IMPLEMENT_DEVICE_METHOD(LicServer,logoutApp)
{
        
    SLicMng mng;
    int ir;
    SAppInfo info;
    JsonParser jp(params),jr;
    qDebug() << "==================in server logOut parms = " << params[APP_PID].asString().c_str();
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
    #if 1
     qDebug() << "  parms = "<<jp.get(APP_VENDER) <<jp.get(APP_PACKAGE) <<jp.get(APP_VERSION) << jp.get(APP_IP) << jp.get(APP_PID);
    #endif
     
//do
    qDebug() << "logout start";
    ir = mng.logoutApp(info);
    qDebug() << "logout end ="<< ir;
    
    QString str;
    if (ir <0)  jr.set(RET_STATUS,RET_ERR);
    else
    {
        jr.set(RET_STATUS,RET_OK);
    }
//out:
    result = jr.getJ();
    qDebug() << "logoutApp in server result = " << result[RET_STATUS].asString().c_str();

    return tetris::device::method_finished;
}
IMPLEMENT_DEVICE_METHOD(LicServer,heartBeat)
{
    SLicMng mng;
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
    ir = mng.heartBeat(info);
    
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
IMPLEMENT_DEVICE_METHOD(LicServer,borrow)
{    
    SLicMng mng;
    int ir;
    QString licStr,proof;
    JsonParser jp(params),jr;
    qDebug() << "=======================in server borrow parms = " << params[CMD_PARAM].asString().c_str();
//in
    licStr = jp.get(CMD_PARAM);// we borrow CMD_PARAM
    //qDebug() << "in server liStr = " << licStr;
//do
    ir = mng.borrowFileStr(licStr,proof);

    if (ir <0)  jr.set(RET_STATUS,RET_ERR);
    else
    {
        jr.set(RET_STATUS,RET_OK);
        jr.set(RET_MESSAGE,proof);
    }
//out:
    result = jr.getJ();
    //qDebug() << "load file in server result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
}
IMPLEMENT_DEVICE_METHOD(LicServer,borrowReturn)
{
    SLicMng mng;
    int ir;
    QString proof;
    JsonParser jp(params),jr;
    qDebug() << "=======================in server borrowReturn parms = " << params[CMD_PARAM].asString().c_str();
//in
    proof = jp.get(CMD_PARAM); 
    //qDebug() << "in server liStr = " << licStr;
//do
    ir = mng.borrowReturn(proof);

    if (ir <0)  jr.set(RET_STATUS,RET_ERR);
    else
    {
        jr.set(RET_STATUS,RET_OK);
    }
//out:
    result = jr.getJ();
    //qDebug() << "load file in server result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
}
IMPLEMENT_DEVICE_METHOD(LicServer,report)
{ 
    SLicMng mng;
    //int ir;
    QString pStr,pStr1,rStr;
    QStringList plist,alist,slist;
    JsonParser jp(params),jr;
//in
    pStr = jp.get(REPORT_PARAM);
    pStr1 = jp.get(REPORT_PARAM_SINGLE);
    qDebug() << " ====================in server report p1,p2 =" << pStr << pStr1;
//do
    if (pStr == REPORT_PARAM_PACKAGE) 
    {
        if(pStr1 == "")
            plist = mng.reportPackage(); 
        else
            plist = mng.reportPackage(pStr1); 

        slist = plist;
    }

    else if (pStr == REPORT_PARAM_APP) 
    {
        if(pStr1 == "")
            alist = mng.reportApp();
        else
            alist = mng.reportApp(pStr1);

        slist = alist;

    }
    else if (pStr == REPORT_PARAM_ALL) 
    {
        plist = mng.reportPackage(); 
        alist = mng.reportApp();
        slist = plist + alist;

    }
    // qDebug() << " slist.size =" <<slist.size();
    
    rStr = "";
    if (slist.size() >0)  rStr = slist.join("\n");
     
    jr.set(RET_STATUS,RET_OK);
    jr.set(RET_MESSAGE,rStr);
//out:
    result = jr.getJ();
   // qDebug() << " report in server  result=" << result.toStyledString().c_str();

    return tetris::device::method_finished;
}
IMPLEMENT_DEVICE_METHOD(LicServer,shutDown)
{
    SLicMng mng;
    int ir;
    QString licStr;
    JsonParser jp(params),jr;
    qDebug() << "===========================in server shutDown = " << params[CMD_PARAM].asString().c_str();
//in
    //licStr = jp.get(CMD_PARAM);
    //qDebug() << "in server call before" ;
//do
    _down = 1;
    qDebug() << "in server call after _down = " << _down ;

  //  if (ir <0)  jr.set(RET_STATUS,RET_ERR);
  //  else
        jr.set(RET_STATUS,RET_OK);
//out:
    result = jr.getJ();
   //qDebug() << "shutdown in server result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
     
}
#if 0
IMPLEMENT_DEVICE_METHOD(LicServer,checkUUID)
{
    SLicMng mng;
    int ir;
    QString para;
    JsonParser jp(params),jr;
    qDebug() << "===============in server checkUUID parms = " << params[CMD_PARAM].asString().c_str();
//in
    para = jp.get(CMD_PARAM);// we borrow CMD_PARAM
    //qDebug() << "in server liStr = " << licStr;
//do
    if (para == "1")  
        mng.setCheckUUID(true); 
    else
        mng.setCheckUUID(false); 
 //

        jr.set(RET_STATUS,RET_OK);

        if (mng.isCheckUUID()) para = "1";
        else para = "0";
        jr.set(RET_MESSAGE, para); 
//out:
    result = jr.getJ();
    //qDebug() << "load file in server result = " << result[RET_STATUS].asString().c_str();
    return tetris::device::method_finished;
}
#endif
