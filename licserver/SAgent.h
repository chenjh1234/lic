#ifndef SAGENT_H
#define SAGENT_H
#include "SAppInfo.h"
#include "SAppMng.h"
#include "SLog.h"
#include "ConfigMng.h"
//#include "ConfigMng.h"
#include <QTextStream>
#include <QStringList>
#include <QMutex>
#include <QFile>
//#include "http_call.hpp"
//#include "comDefs.h"
//#include <json/json.h>
#define AGENT_LOG_FILE "/tmp/licagent.log"
#define AGENT_DB_FILE "/tmp/licagentApp.db"
#define AGENT_CONFIG_FILE "/tmp/licagentConfig.db"

class SAgent
{
public:
    SAgent();
    ~ SAgent();
//login logout
// login:

    int registerApp(SAppInfo &info);
    int unRegisterApp(SAppInfo &info);
    int unRegisterApp(QString pack,QString app);

    bool  isLoginConfig(SAppInfo &msg);
    bool  isLogoutConfig(SAppInfo &msg);
// db:
    QString dbFile(); 
    void setDbFile(QString file);
    int saveDB();
    int loadDB();
    int saveDBApp(QDataStream& ds);
    int loadDBApp(QDataStream& ds);
//log
    QString logFile(); 
    void setLogFile(QString file);
    void log(QString s1,QString s2="",QString s3="",QString s4="");
//hb: 
    int appHB(SAppInfo& app);
 //   int checkHB(long intvs);
 //   int checkHB(QString pack,long intvs);
// kill app
    int killApp(SAppInfo &info);
//
    int startUp();
    int down();
// convert"
    QString view(QString pack = "");
    QString viewPack(QString pack);
// 
    int size();
    bool isPack(QString packid);
    bool isApp(QString packid,QString appid);

    ConfigMng  _config;
    QMap<QString,SAppMng *> packMng;

     void setIP(QString ip);
     QString IP();
     void setPort(int p);
     int port();
    

   

private:
    SAppMng  * getAppMng(QString packid);
    bool isAppMng(QString packid);
    int removePack(QString packid);
    void init();
 
    
     
    QString  _dbFile, _logFile;
    QMutex _lockApp; // login,unlogin;
    QMutex _lockLog;
    SLog _log;
    LFileDate _dt;
    QString _IP;
    int _port;



};


#endif

