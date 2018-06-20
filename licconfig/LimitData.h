#ifndef LIMIT_DATA_H
#define LIMIT_DATA_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>
#include <QMutex>
// status  limitData
#define DSTAT_OK 0 
#define DSTAT_FULL 1
#define DSTAT_REMOVE -1

#define DSTAT_FAILED -1

#define LOGIN_OK 0 
#define LOGIN_FAILED -1
#define LOGIN_PASS 0 
#define LOGIN_NONE -100 
 
class LimitData
{
public:
    LimitData();
    ~LimitData();
	int _limit;
    int _count;
    int _status;//0ok,1,full,-1,remove;

    QMutex _lock;// we do lock in login ,unlogin;

    bool isFull();
    bool isRemove();
    int setRemove();// set status to STAT_REMOVE

    bool canLogin(int n);
    bool canLogin();

    int login();
    int login(int n);// we lock and unload to fit multi thread:

    int unlogin();
    int unlogin(int n);// we lock and unload to fit multi thread:

    int saveDB(QDataStream &ds);
    int loadDB(QDataStream &ds);

    // set and manage the status;  mybe the count is >0  for reload config file;
    int setLimit(int n);
    int setStatus(int n);
    int limit();
    int count();
    int status();
    
private:
    void init();
   
};
 

#endif

