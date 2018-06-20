
#include "LimitData.h"
// LimitData=============================
LimitData::LimitData()
{
   init();
}
LimitData::~LimitData()
{
}
void LimitData::init()
{
   setLimit(0);
   _count = 0;
   setStatus(DSTAT_OK);
}
bool LimitData::isFull()
{
   bool f;
   f = false;
   if (_count >= limit()) f = true;
   if (status()!= DSTAT_REMOVE) 
   {
       if (f) setStatus(DSTAT_FULL);
       else setStatus(DSTAT_OK);
   }
   return f;

}
bool LimitData::isRemove()
{

   return status() == DSTAT_REMOVE;

}
// canLogin
bool LimitData::canLogin()
{
   return canLogin(1);
}
bool LimitData::canLogin(int n)
{
   return _count + n <= _limit;
}
// login:
int LimitData::login()
{
   return login(1);

}
int LimitData::login(int n)
{ 
    // lock;
    if (!canLogin( n))
    {   
        //unloak; 
        return LOGIN_FAILED;
    }

    _lock.lock();
    _count += n;
   isFull(); // set status;
   _lock.unlock();
   //unloak;
   return _count;

}
/// unlogin
int LimitData::unlogin()
{
   return unlogin(1);
}
int LimitData::unlogin(int n)
{
    //lock;
    _lock.lock();
    _count -= n;
    if (_count < 0)  qDebug() << "error------------count <0 ;in unlogin?? ";
    isFull(); // set status;
   //unlock;
    _lock.unlock();
   return _count;
}
/// setlimit
int LimitData::setLimit(int n)
{
   _limit = n;
   if (status() == DSTAT_REMOVE) setStatus(DSTAT_OK);
   isFull(); // set status;
   return _limit;
}
///setRemove
int LimitData::setRemove()
{
   setStatus(DSTAT_REMOVE);
   return _status;
}

int LimitData::setStatus(int n)
{ 
    _status = n;
    return _status;
}
int LimitData::limit()
{
    return _limit;
}
int LimitData::status() 
{ 
    return _status;
}
int LimitData::count()
{
    return _count;
}
int LimitData::saveDB(QDataStream &ds)
{
    ds << _limit;
    ds << _count;
    ds << _status;
    return DSTAT_OK;
}
int LimitData::loadDB(QDataStream &ds)
{
    ds >> _limit;
    ds >> _count;
    ds >> _status;
     return DSTAT_OK;
}
