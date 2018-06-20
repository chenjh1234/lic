
#ifndef APP_CLIENT_H__ 
#define APP_CLIENT_H__ 

#include "startClient.h"
#include "LLicEncrypt.h"

int should_exit = 0;
void sighandler(int /*signum*/)
{ // should_exit = 1;
   exit(1);
} 

class AppClient
{
public:
    AppClient()
    {
        signal (SIGINT, sighandler);
    };
    ~AppClient(){};
    QStringList login(QString vender,QString packName,QString version,int number=1,QString appname="")
    {
         return SC.client->loginApp(vender,packName,version,number,appname);
    }
    int logout(QString vender,QString packName,QString version)
    {
        return SC.client->logoutApp(vender,packName,version);
    }
    // if only one package license applied:
    int logout()
    {
        return SC.client->logoutApp();
    }
    bool checkValid(QStringList slist,QString pub,QString seed)
    {
        LLicEncrypt lic;
        QString  sign, sid;
        sign = slist[0];
        sid = slist[1];
        return lic.verifyVenderSeed(pub,sign,seed,sid);
    }
    bool checkValid(QString pub,QString sign,QString seed,QString sid)
    {
        LLicEncrypt lic;
        return lic.verifyVenderSeed(pub,sign,seed,sid);
    }
    bool loginCheck(QString pub,QString seed,QString vender,QString packName,QString version,int number=1,QString appname="")
    {
         QStringList slist;
         slist = SC.client->loginApp(vender,packName,version,number,appname);
         if (slist.size() < 2) return false; 
         return checkValid(slist,pub,seed);
    }
    StartClient SC;
};
#endif

