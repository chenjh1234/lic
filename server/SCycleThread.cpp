#include "SCycleThread.h"

SCycleThread::SCycleThread():QThread()
{
    rHB.setRun(HEARTBEAT_INT);
    rDbSave.setRun(DB_SAVE_INT);
    rDayCheck.setRun(DAY_CHECK_INT);
    rAppExp.setRun(APP_EXP_INT);
    setDown(0);// not down
    setRole(ROLE_SERVER);
    init();
    
}
int SCycleThread::init()
{
     
    _dbSave =  lfd.sEP(); 
    _appExp =  lfd.sEP();
     //start of the day
  #ifdef TEST_CYCLE
    _dayCheck = lfd.sEP();
  #else
    _dayCheck =  lfd.curD().toLong(); 
  #endif
  return 0;
 
}
int SCycleThread::setDown(int d)
{
    _down = d;
    return _down;
}
int SCycleThread::setDown()
{
    _down = CYCLE_DOWN;
    return _down;
}
bool SCycleThread::isDown()
{
    return _down == CYCLE_DOWN;
}
void SCycleThread::run()
{
    if (role() == ROLE_SERVER) 
        runServer();
     if (role() == ROLE_AGENT) 
        runAgent();
}
void SCycleThread::runServer()
{
    long dc;//daycheck
    int ic;
    init();

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
        //HB:
        runHB();
        rHB.wait();
        qDebug() << "HB OK"<< lfd.sEP()<<lfd.curDT();
        // dbsave: 
        if (lfd.sEP() - _dbSave >= DB_SAVE_INT) 
        {
            qDebug() << "DBSAVE start";
            _dbSave = lfd.sEP();
            runDBSave();
            rDbSave.wait();
            qDebug() << "DBSAVE OK"<< lfd.sEP()<<lfd.curDT();
        }
        //appExp:
        // wait HB, DBSAVE complete
        
        
        if (lfd.sEP() - _appExp >= APP_EXP_INT) 
        {
             qDebug() << "appEXP start";
            _appExp = lfd.sEP();
            runAppExp();
             rAppExp.wait();
             qDebug() << "appEXP end"<< lfd.sEP()<<lfd.curDT();
        }
        //day check: packExp;log
        #ifdef TEST_CYCLE
             dc = lfd.sEP();
        #else
             dc =  lfd.curD().toLong(); 
        #endif
          // waitAPP exp complete
            

        if (dc - _dayCheck >= DAY_CHECK_INT) 
        {
            qDebug() << "day check start"<< lfd.sEP()<<lfd.curDT();
            _dayCheck = dc;
            runDayCheck();
            rDayCheck.wait();
            qDebug() << "day check end"<< lfd.sEP()<<lfd.curDT();
        }
        // wait for day check complete:
       
 
    }
    rHB.wait();
    rDbSave.wait();
    rAppExp.wait();
    rDayCheck.wait();

    //qDebug() <<  "i go out----------------------";
}
void SCycleThread::runAgent()
{
    long dc;//daycheck
    int ic;
    init();

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
        //HB:
       runHB();
       rHB.wait();
      //  qDebug() << "HB OK"<< lfd.sEP()<<lfd.curDT();
        // dbsave: 
        if (lfd.sEP() - _dbSave >= DB_SAVE_INT) 
        {
            qDebug() << "DBSAVE start";
            _dbSave = lfd.sEP();
            runDBSave();
            rDbSave.wait();
            qDebug() << "DBSAVE OK"<< lfd.sEP()<<lfd.curDT();
        }
 
    }
    rHB.wait();
    rDbSave.wait();
//   rAppExp.wait();
//    rDayCheck.wait();

    //qDebug() <<  "i go out----------------------";
}
// -------------run ------------------
int SCycleThread::runHB()
{
    
   rHB.start();
   
   return 1;
}
int SCycleThread::runDBSave()
{
    rDbSave.start();
    return 1;
}
int SCycleThread::runDayCheck()
{
    rDayCheck.start();
    return 1;

}
int SCycleThread::runAppExp()
{
   rAppExp.start();
   return 1; 
}

void SCycleThread::setRole(int role)
{
    _role = role;
    rHB.setRole(role);
    rAppExp.setRole(role);
    rDbSave.setRole(role);
    rDayCheck.setRole(role);
}
int SCycleThread::role()
{
    return _role;
}

 
//============runThread========================
runThread::runThread():QThread()
{
#ifdef TEST_CYCLE 
    stt = lfd.sEP();
#endif
}
int runThread::setRun(int mode)
{
    _who = mode;
    return mode;
}

QString  runThread::who()
{
    QString str;
    if (_who == HEARTBEAT_INT)  str = "HB";
    if (_who == DB_SAVE_INT)  str = "DBSAVE";
    if (_who == DAY_CHECK_INT)  str = "PACK_EXP";
    if (_who == APP_EXP_INT)  str = "APP_EXP";
    return str;
}
void runThread::run()
{
    if (role() == ROLE_SERVER) 
        runServer();
     if (role() == ROLE_AGENT) 
        runAgent();
}
void runThread::runServer()
{
    SLicMng mng;
    int i;
    qDebug() << "who = " << who() ;

#ifdef TEST_CYCLE 
    qDebug() << "who = " << who()  << lfd.sEP() - stt;
    ilist << lfd.sEP() - stt;
    //return;
#endif

    if (_who == HEARTBEAT_INT) 
    {
        i = mng.data->checkHB(HEARTBEAT_INT);
    }
    #if 1
    else if (_who == DB_SAVE_INT) 
    {
        i = mng.data->saveDB();   
        mng.data->_config.saveDB();    
    }
    else if (_who == DAY_CHECK_INT) 
    {
        i = mng.data->changeLogNames();
        i = mng.data->checkPackExp();
    }
    else if (_who == APP_EXP_INT) 
    {
        i = mng.data->checkAppExp(APP_EXP_INT);
    }
    #endif
    #ifdef TEST_CYCLE 
    nList << i;
    #endif
    qDebug() << "thread ok ===" << who() << i;
}
//========agent:
void runThread::runAgent()
{
    SAgent mng;
    int i;
    qDebug() << "who = " << who() ;

#ifdef TEST_CYCLE 
    qDebug() << "who = " << who()  << lfd.sEP() - stt;
    ilist << lfd.sEP() - stt;
    //return;
#endif
    #if 1
    if (_who == DB_SAVE_INT) 
    {
        i = mng.saveDB();   
        mng._config.saveDB();    
    }
    #endif
    #ifdef TEST_CYCLE 
    nList << i;
    #endif
    qDebug() << "thread ok ===" << who() << i;
}
void runThread::setRole(int role)
{
    _role = role;
}
int runThread::role()
{
    return _role;
}
      
