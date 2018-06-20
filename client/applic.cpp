
#include "startClient.h"
#include "LLicEncrypt.h"
#include <QString>
#include <QStringList>
#include "applic.h"

StartClient *SC =NULL;
#define START_CLIENT if(SC == NULL) SC = new StartClient();
/**
 * login to lic servers
 * 
 * @author cjh (10/24/2017)
 * 
 * @param vender  vendername
 * @param pack  package name
 * @param version version of the package
 * @param number  number of license applied
 * @param appname applicationname 
 * @param sign output the vender sign,caller allocate space 300
 *             bytes
 * @param sid  output the server mid,caller allocate space 40
 *             bytes
 * 
 * @return int <0 error,>=0 OK;
 */
int loginlic(const char *vender,const char* pack,const char *version,int number,char* appname,char *sign,char *sid)
{
    QStringList slist;
    QString v,p,ver,n,app,sg,sd;
    v = vender;
    p = pack;
    ver = version;
    app =appname;

    START_CLIENT;
    
    slist = SC->client->loginApp(v, p, ver, number, app); 
    if (slist.size() <2) return -1;
    memcpy(sign,slist[0].Q2CH,slist[0].length());
    sign[slist[0].length()] =0;
    memcpy(sid,slist[1].Q2CH,slist[1].length());
    sid[slist[1].length()] =0;
    return 1;  
}
/**
 * logout from lic server
 *  
 * @param vender vender name
 * @param pack  package name
 * @param version  version of the package
 * 
 * @return int ok: >=0,Err <0;
 */
int logoutlic(const char *vender,const char* pack,const char *version)
{
    int i;
    QString v,p,ver;
    v = vender;
    p = pack;
    ver = version;
    if (SC == NULL) return -1;
    i = SC->client->logoutApp(v, p, ver); 
    return i;

}
/**
 * 
 * logout from licserver,
 * 
 * @return ok: >=0,Err <0;
 */
int logoutlic() 
{
    if (SC == NULL) return -1;
    return SC->client->logoutApp();
}
/**
 * check if login return(sign,serverid) is valid; 
 *  
 * @param sign  login returnd
 * @param sid   login returnd 
 * @param pub   public key of the vender
 * @param seed  seed string of the vender
 * 
 * @return bool true: if valid ,false: if incalid;
 */
bool checklic(char * sign,char *sid, char *pub,char *seed)
{
    LLicEncrypt lic;
    QString sn,ssid,p,sd;
    p = pub;
    sn = sign;
    sd = seed;
    ssid = sid;

    return lic.verifyVenderSeed(p,sn,sd,ssid);//pub,sign,seed,sid
}
/**
 * login to license  server and check if the return is valid;
 * 
 * @param vender vender name
 * @param pack package name
 * @param version version of the package
 * @param number number of license appied
 * @param appname application name
 * @param pub public key of the vender
 * @param seed seed string of the vender
 * 
 * @return int 
 */
bool logincheck(const char *vender,const char * pack,const char *version,int number,char* appname,char *pub,char *seed)
{
    QStringList slist;
    QString v,p,ver,n,app,pb,sd;
    v = vender;
    p = pack;
    ver = version;
    app =appname;
    START_CLIENT;

    slist = SC->client->loginApp(v,p,ver,number,app);
    if (slist.size() <2) return false;

    LLicEncrypt lic;
    sd = seed;
    pb = pub;
    return lic.verifyVenderSeed(pb,slist[0],sd,slist[1]);//pub,sign,seed,sid
}
