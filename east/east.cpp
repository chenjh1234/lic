#include "east.h"
#include "applic.h"
#include <string>
#include <fstream>
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <vector> 
#include <iostream> 
//#include "appInclude.h"
using namespace std;
#define EAST_OK  1
#define ERR_APPNAME  -1
#define ERR_LOGIN -2
#define ERR_CHECK  -3
#define ERR_LOGOUT -4

// the vender seed, publicKey;
#define VENDER_NAME "geoeast"
#define PACK_VERSION "1.0"
#define VENDER_SEED    "0123456789" //
unsigned char pubkey[] = \
      "-----BEGIN PUBLIC KEY-----\n"\
      "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDLpkCvoO/bC7bY2iAKK/LX+yMK\n"\
      "CnL6FpatrM9Yk59ce7/S/T7OqgRhZYMIhGU6rJOB7finilEYDAtt5OJYUqNF+cS0\n"\
      "vZENDDsWWPvUYcYEcUurXN/G0PbcTjU90mqcNXx5GC9AQQeV3PYiqzHhYoJqKECB\n"\
      "ibzelvxMWGgLUZM+swIDAQAB\n"\
      "-----END PUBLIC KEY-----\n";

#if 0
int login_lic( char *vender,char* pack,char *version,int number,char* appname,char *sign,char *sid);
int logout_lic( char *vender,char* pack, char *version);
int logout_lic();
int logout_licf();
bool check_lic(char * sign,char *sid, char *pub,char *seed);
bool login_check(char *vender,char* pack,char *version,int number,char* appname,char *pub,char *seed);
#endif
/**
 * 
 * 
 * @author cjh (9/27/2018)
 * 
 * @param packname 
 * @param num 
 * 
 * @return int 
 */
int east_login (char *packname,int &num,char *appn )
{
    char sign[1024],sid[40];
    string appname;

    int i,number;  
    appname = appn;
//parameters:
    if (strlen(packname) <=0)  
    {
        cout << "pack name cannot be NULL\n";
        return ERR_APPNAME;
    }
    number = num;
    cout << "appname = "<< appname <<number << endl;
 
// start 
    char *seed,*pub,*vender,*version;
    pub = (char *)pubkey;
    seed = VENDER_SEED;
    vender = VENDER_NAME;
    version = PACK_VERSION; 
 
    i = login_lic(vender,packname,version,number,(char *)appname.c_str(),sign,sid);
    if (i<0) return ERR_LOGIN;
     
    cout << "sid return = "<<sid <<endl;
     
    if(!check_lic(sign,sid,pub,seed))
    {
        cout << "app checkValid failed\n";
        return ERR_CHECK;
    }
    cout <<"east login return ok =" << EAST_OK <<endl;
    
    return EAST_OK;
}
  
int east_logoutf()
{ 
    int i;    
    i = logout_licf();
    if (i < 0) 
    {
            cout << "logout eror \n";
            return ERR_LOGOUT;
    }
     cout <<"east logout return ok =" << EAST_OK <<endl;
    return EAST_OK;
}
 

int east_logout (char *pack)
{ 
    int i;    
    char *seed,*pub,*vender,*version;
    version = PACK_VERSION;
    vender = VENDER_NAME;
    cout << "len of pack = " <<strlen(pack) <<pack <<endl ;
    if (strlen(pack)<=0)  return  ERR_LOGOUT;

    i = logout_lic(vender, pack, version); 

    if (i < 0) 
    {
            cout << "logout eror \n";
            return ERR_LOGOUT;
    }
    return EAST_OK;
}
