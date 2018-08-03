#ifndef SCYCLE_THRAED_H
#define SCYCLE_THRAED_H


#include "LFileDate.h"
#include "SLicMng.h"
#include "SAgent.h"
#include <QThread>

#define TEST_CYCLE1 // if unit TEST : TEST_CYCLE;

#define CYCLE_DOWN -1
#define ROLE_SERVER 0
#define ROLE_AGENT 1

 
#ifdef TEST_CYCLE   // test:
#define HEARTBEAT_INT 2   //3M   server check heatBet of each app;
#define DB_SAVE_INT 4   // 1hour  save DB to disk
#define DAY_CHECK_INT 6 // 1Day,check if package is expired,
#define APP_EXP_INT 8 // 7Days, we log warning message if app run coninuous more than 7days
#else
#define HEARTBEAT_INT 3*60  //3M   server check heatBet of each app;
#define DB_SAVE_INT 60*60   // 1hour  save DB to disk
#define APP_EXP_INT 7*24*60*60 // 7Days, we log warning message if app run coninuous more than 7days

//#define PACK_EXP_INT 24*60*60 // 1Day,check if package is expired,start from minight  
#define DAY_CHECK_INT 1 // 1Day,check if package is expired,if log filename  changed start from minight                            
//#define LOG_NAME_INT 1*24*60*60 // every day we check it:

#endif
// run thread:============================

class runThread : public QThread
{
public:
    runThread();
    int setRun(int who);
    QString who();

#ifdef TEST_CYCLE 
    long stt;
    QList<int > ilist;// counter calls
    QList<int> nList;// return int each in each call
#endif
    void setRole(int role);
    int role();
private:
    void run();
    void runServer();
    void runAgent();
    int _who;
    LFileDate lfd;
     int _role;
    
   
};

// ScycleThread:====================================
class SCycleThread : public QThread
{
public:
    SCycleThread();
    int runHB();
    int runDBSave();    
    int runAppExp();
    int runDayCheck();


    int setDown(int d);
    int setDown();
    bool isDown();
    void setRole(int role);
    int role();

    runThread rHB,rAppExp,rDbSave,rDayCheck;
private:
    int init();
    void run();
    void runServer();
    void runAgent();
    //void getStart();
    LFileDate lfd;
    long _hb,_dbSave,_appExp,_dayCheck;
    int _down;
    int _role;
    
};

#endif
