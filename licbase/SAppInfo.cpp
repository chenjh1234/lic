#include "SAppInfo.h"

SAppInfo::SAppInfo():LInfo()
{
    init();
}
SAppInfo::~SAppInfo()
{

}

void SAppInfo::init()
{
    err = "";
    number = 0;
    hbstat = 0;
    start = 0;
    end = 0;
    ret = 1;

}
bool SAppInfo::isExpired(long intvs)
{
    LFileDate fd;
    long t;
    t = fd.sEP();
    return  (t - start >= intvs);
}

int SAppInfo::appHB()
{
    hbstat = fd.sEP();
    return hbstat;
}
bool SAppInfo::checkHB(long intvs)
{
    QString s;
    s = get(APP_AGENT).toString();
    qDebug() << "app agent check s =" << s;
    if (!s .isEmpty())  return false; //  donot checkHB:    is agent  check HB

    if (hbstat == 0) 
    {
        hbstat = fd.sEP();
    }

    if (fd.sEP() - hbstat >= intvs ) return true;
    else return false;
}
QString SAppInfo::encodePackageID()
{
   QString str, dl;
   QString package,version,vender;
   vender = get(APP_VENDER).toString();
   package = get(APP_PACKAGE).toString();
   version  = get(APP_VERSION).toString();
   dl = "_";
   str = vender + dl + package + dl +   version;
   return str;
}
 
QString SAppInfo::encodeAppID()
{
   QString ip,pid;
   QString str, dl;
   pid = get(APP_PID).toString();
   ip = get(APP_IP).toString();
   dl = "_";
   str = ip + dl + pid;
   return str;
}
int SAppInfo::encode(QDataStream& ds)
{
    LInfo::encode(ds);
    ds << packid;
    ds << appid;
    ds << user;
    ds << number;
    ds << qint64(hbstat);
    ds << qint64(start)<<qint64(end);
    ds << rtype;
    ds << err;
    ds << ret;
    return 1;


    #if 0
        QString packid;// vender+package+version
    QString appid;// ip+pid;
    QString user;// ip+pid;
    int number;
    long hbstat;
    long start,end;
    QString rtype;//APP_TYPE:PTYPE_NODE:TYPE_TASK:PTYPE_USER
    QString err;
    int ret;
    #endif
}
int SAppInfo::decode(QDataStream& ds)
{
    LInfo::decode(ds);
    qint64 hb,s,e;
    ds >> packid;
    ds >> appid;
    ds >> user;
    ds >> number;
    ds >> hb;
    ds >> s >> e;
    ds >> rtype;
    ds >> err;
    ds >> ret;

    hbstat = hb;
    start  = s;
    end = e;

    hbstat = 0;
    return 1;

}
