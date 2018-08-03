#include "SAgent.h"
#include <iostream>
#include "testG.h"
#include <QString>
#include <QStringList>
#include <QThread>
//#include <http_call.hpp>
#include "LFileDate.h"
#include "SAppInfo.h"
#include "comDefs.h"
#include "jsonParser.hpp"
using namespace std;
 
SAgent agt;
 SAppInfo & getApp(QString pack,QString pid1)
 {
      int i;
    QString str;
    SAppInfo *info;
    info = new SAppInfo();
   
   Json::Value params, result;
   JsonParser jp, jr;
   QString vender,packName,version,ip,hostname,appname,user,pid,number;
   vender = "bgp";
   packName = pack;//"geoeast";
   version = "1.0";
   number = "1";
   ip = "192.168.1.1";
   hostname = "bgpcjh";
   appname = "myApp";
   user= "cjh";
   pid = pid1;//"12345";

   info->set(APP_VENDER,vender);
   info->set(APP_PACKAGE,packName);
   info->set(APP_VERSION,version);
   info->set(APP_NUMBER,number);
   info->set(APP_IP,ip);
   info->set(APP_HOST,hostname);
   info->set(APP_NAME,appname);
   info->set(APP_USER,user);
   info->set(APP_PID,pid);
   qDebug() << "return of getApp";
   return *info;
 
 }

U_START(login)
    int i;
    QString str,packName,packid,packid1,appid,appid1;
    SAppInfo info;
    qDebug() << " start---:";
    info = getApp("geoeast","12345");
    qDebug() << info.getText();
    packid = info.encodePackageID();
    appid =  info.encodeAppID();
// login1:
    qDebug() << " test gb:";
   i = agt.registerApp(info);
   PR("i = "+STR(i));
   GT(i,0);//1
   i=agt.size();
   qDebug() << "size = " << agt.size();
   EQ(i,1);//2
// login 2:
  info = getApp("geoeast111","12345");
  packid1 = info.encodePackageID();
  i = agt.registerApp(info);
  GT(i,0);//3
  i=agt.size();
  EQ(i,2);//4
  // login 2-1:
  info = getApp("geoeast111","54321");
  i = agt.registerApp(info);
  appid1 =  info.encodeAppID();
  GT(i,0);//5
  i=agt.size();
  EQ(i,2);//6
  i = agt.packMng[packid1]->size();// apps of geoeast111=2
  EQ(i,2);//7
   i= agt.packMng[packid]->size();// apps og geoeast: =1
  EQ(i,1);//8

 
U_END

U_START(unlogin)
    int i;
    QString str,packName,packid,packid1,appid,appid1;
    SAppInfo info;

// logout:----------------------------------------------

   info = getApp("geoeast","12345");
   packid = info.encodePackageID();
   appid =  info.encodeAppID();

   i = agt.unRegisterApp(info);// remove of all geoeast apps=1
   GT(i,0);//size();//9
   i=agt.size();
   qDebug() << "size = " << agt.size();
   EQ(i,1);//10

  info = getApp("geoeast111","12345");
   packid1 = info.encodePackageID();
   i = agt.unRegisterApp(info);// remove of all geoeast111 apps=1 now
   GT(i,0);//size();//11
   i=agt.size();
   qDebug() << "size = " << agt.size();
   EQ(i,1); //12
   i= agt.packMng[packid1]->size();// apps of geoeast111=1
   EQ(i,1);//13
   qDebug() << "unregister the last: ";

   info = getApp("geoeast111","54321");
   appid1 =  info.encodeAppID();
   qDebug() << "unregister the last: ";
   i = agt.unRegisterApp(info);// remove of all geoeast111 apps=0 now
   GT(i,0);//size();//14
   i=agt.size();
   qDebug() << "size = " << agt.size();
   EQ(i,0);//15
//errs
   i = agt.unRegisterApp(info);// err happens
   LT(i,0);//size();//16
 

U_END
U_START(dbsave)
    int i;
    QString str,packName,packid,packid1,appid,appid1;
    SAppInfo info;
    i=agt.saveDB();
    qDebug() << "dbsave ret= " << i;
    GT(i,0)
 U_END
U_START(dbload)
    int i;
    QString str,packName,packid,packid1,appid,appid1;
    SAppInfo info;
    i=agt.loadDB();
    qDebug() << "dbload ret= " << i;
    GT(i,0)
    i=agt.size();
    EQ(i,2);
    str = agt.view();
    QStringList slist;
    slist = str.split("\n");
    for (i = 0; i <slist.size() ; i++ ) 
    { 
        qDebug() << slist[i];
    }
    
 U_END
M_START
   // cf.setDbFile("dbConfig.db");
     U_TEST(login)      
     U_TEST(dbsave)
     U_TEST(unlogin)   
     U_TEST(dbload)  
    // U_TEST(login)//  login and unlogin: include full, and failed caused by apply resource number more than limit;
  //  U_TEST(db);// save db,load db,;ok
   // U_TEST(lock) //lock: multithread access the config data;
   // U_TEST(loadnull)
M_END

