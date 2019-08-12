
#include "startClient.h"
#include "LLicEncrypt.h"
#include <QString>
#include <QStringList>
#include "applic.h"

StartClient *SC =NULL;
#define START_CLIENT if(SC == NULL) SC = new StartClient();
/**
 *@brief 请求注册软件许可资源 
 * login to lic servers
 * 
 * @author cjh (10/24/2017)
 * 
 * @param vender  开发商名名称 
 * vendername
 * @param pack  软件包名称 
 * package name
 * @param version 软件包版本号 
 * version of the package
 * @param number  请求的许可数量 
 * number of license applied
 * @param appname 应用程序名称 
 * applicationname 
 * @param sign 
 *             返回：开发商签名信息（需要300个字节空间）息
 *             output the vender sign,caller allocate space 300
 *             bytes
 * @param sid 返回：服务器ID（需要40个字节空间） 
 *               output the server mid,caller allocate space 40
 *               bytes

 * 
 * @return 返回：int <0 error,>=0 OK;
 */
int login_lic(char *vender,char* pack,char *version,int &number,char* appname,char *sign,char *sid)
{
    QStringList slist;
    QString v,p,ver,app,sg,sd;
    int i;
    int n; 
    v = vender;
    p = pack;
    ver = version;
    app =appname;
    n = number;

        START_CLIENT;

    slist = SC->client->loginApp(v, p, ver, n, app); 
    if (slist.size() <2) return -1;
    memcpy(sign,slist[0].Q2CH,slist[0].length());
    sign[slist[0].length()] =0;
    memcpy(sid,slist[1].Q2CH,slist[1].length());
    sid[slist[1].length()] =0;
    return 1;  
}
/**
 *@brief 注销登记的软件许可资源  
 * logout from lic server
 *  
 * @param vender 开发商名名称 
 * vender name
 * @param pack  软件包名称 
 * package name
 * @param version  软件包版本号 
 * version of the package
 * 
 * @return  返回：int ok: >=0,Err <0;
 */
int logout_lic(char *vender,char* pack,char *version)
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
 * @brief 
 *        无从参数注销登记的软件许可资源，（参数来源于最近一次login_lic的参数）
 *  
 *
 * 
 * @return  返回：int ok: >=0,Err <0;
 */
int logout_licf() 
{
    if (SC == NULL) return -1;
    return SC->client->logoutApp();
}
/**
 * @brief 
 * 检查login_lic返回的开发商签名信息，服务器ID是否合法。
 * check if login return(sign,serverid) is valid; 
 *  
 * @param sign  开发商签名信息 
 *  login returnd
 * @param sid   服务器ID 
 *  login returnd 
 * @param pub   开发商的公钥串 
 *  public key of the vender
 * @param seed 开发商的私密串  
 *   seed string of the vender
 * 
 * @return 返回：真：成功，假：失败。 
 * ： bool true: if valid ,false: if incalid;
 */
bool check_lic(char * sign,char *sid, char *pub,char *seed)
{
    LLicEncrypt lic;
    QString sn,ssid,p,sd;
    bool b;
    p = pub;
    sn = sign;
    sd = seed;
    ssid = sid;
    cout <<" size of pub = " << p.length() <<endl;
    cout <<" size of sed = " << sd.length() <<endl;
    cout <<" size of sn = " << sn.length() <<endl;
    cout <<" size of ssid = " << ssid.length() <<endl;


    b = lic.verifyVenderSeed(p,sn,sd,ssid);//pub,sign,seed,sid
    cout << "check_lic= " << b <<endl;
    return b;
}
/**
 * @brief 
 * 请求注册软件许可资源,并检查返回的开发商签名信息，服务器ID是否合法。
 *   login to license  server and check if the return is valid;
 * @param vender  开发商名名称 
 * vendername
 * @param pack  软件包名称  
 * package name
 * @param version 软件包版本号  
 * version of the package
 * @param number  请求的许可数量  
 * number of license applied
 * @param appname 应用程序名称   
 * application name 
 * @param pub   开发商的公钥串 
 * public key of the vender
 * @param seed 开发商的私密串  
 *  seed string of the vender

 * @return 返回：真：成功，假：失败。
 * ： bool true: if valid ,false: if incalid; 
 */
bool login_check(char *vender,char * pack,char *version,int &number,char* appname,char *pub,char *seed)
{
    QStringList slist;
    QString v,p,ver,app,pb,sd;
    int n;
    v = vender;
    p = pack;
    ver = version;
    app =appname;
    n = number;

    START_CLIENT;

    slist = SC->client->loginApp(v,p,ver,n,app);
    if (slist.size() <2) return false;

    LLicEncrypt lic;
    sd = seed;
    pb = pub;
    return lic.verifyVenderSeed(pb,slist[0],sd,slist[1]);//pub,sign,seed,sid
}
/**
 * @brief 

 *  report license file loaded
 *   @param licinfo
 *     license file status
 * @return  
 * ：int  : size of information
 */
int  lic_report(char *licinfo)
{
    #define MAX_LEN 10000
    QStringList slist;
   
    int i;
    string  str;
    START_CLIENT;

    slist = SC->client->report();

    for (i = 0; i <slist.size(); i++) 
    {
        str += slist[i].Q2CH;
        str += "\n";
    }
    if (str.length() >= MAX_LEN) 
    {
        return 0;
    }
    memcpy(licinfo, str.c_str(), str.length()); 
    return  str.length();
}
