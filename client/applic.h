
#ifndef APP_LIC_H__ 
#define APP_LIC_H__ 

#define login_lic login_lic__
#define logout_lic logout_lic__
#define check_lic check_lic__
#define login_check login_check__
#define logout_licf logout_licf__

extern "C"
{
    int login_lic(char *vender, char* pack, char *version,int &number,char* appname,char *sign,char *sid);
    int logout_lic(char *vender,char* pack,char *version);
    int logout_licf();
    bool check_lic(char * sign,char *sid, char *pub,char *seed);
    bool login_check(char *vender,char* pack,char *version,int &number,char* appname,char *pub,char *seed);
}
#endif

/**
 * @mainpage  mainpage 
 *  
 *
 *  
    @subpage  简要说明
  
    为应用程序提C及Fortran的的接口\n
    主要包括：注册，注销，注册合法性检查

 *  
   @b functions:
  
    int login_lic()\n
    int logout_lic()\n
    int logout_licf()\n
    bool check_lic()\n
    bool login_check()\n\n
  
    @b appExample:
 *   
 *  @code example apps.cpp
 #include "applic.h"
#include <string>
#include <fstream>
#include <stdio.h>  
#include <stdlib.h> 
#include <unistd.h> 
#include <vector> 
#include <iostream> 

using namespace std;

#define VENDER_SEED    "0123456789" //10 length string
unsigned char pubkey[] = \
      "-----BEGIN PUBLIC KEY-----\n"\
      "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDLpkCvoO/bC7bY2iAKK/LX+yMK\n"\
      "CnL6FpatrM9Yk59ce7/S/T7OqgRhZYMIhGU6rJOB7finilEYDAtt5OJYUqNF+cS0\n"\
      "vZENDDsWWPvUYcYEcUurXN/G0PbcTjU90mqcNXx5GC9AQQeV3PYiqzHhYoJqKECB\n"\
      "ibzelvxMWGgLUZM+swIDAQAB\n"\
      "-----END PUBLIC KEY-----\n";

unsigned char help[] = \
"usage: appt vender package version number time logout\n"\
"p1:vender: vender name\n"\
"p2:package: package name\n"\
"p3:version: version name\n"\
"p4:number: number license apply(1)\n"\
"p5:delay:time in second the app spend(100)\n"\
"p6:if present: we do logout\n";

#if 0
int login_lic( char *vender,char* pack,char *version,int number,char* appname,char *sign,char *sid);
int logout_lic( char *vender,char* pack, char *version);
int logout_licf();
bool check_lic(char * sign,char *sid, char *pub,char *seed);
bool login_check(char *vender,char* pack,char *version,int number,char* appname,char *pub,char *seed);
#endif

main(int argc ,char **argv)
{
    typedef vector<string> stringList;
    char sign[1024],sid[40];
    int i,number;

    string p0,p1,p2,p3,p4,p5,p6,appname;
    p4 ="1";
    p5 ="100";
    p6 ="";
//parameters:
    if (argc <4)  
    {
        cout << help;
        exit(1);
    }
    else 
    {
        p0 = argv[0];
        p1 = argv[1];
        p2 = argv[2];
        p3 = argv[3];
    }
    if (argc >=5)  p4 = argv[4];
    if (argc >=6)  p5 = argv[5];
    if (argc >=7)  p6 = argv[6];

    //slist << p0<<p1<<p2<<p3<<p4<<p5<<p6;
    appname = "apptest";
    sscanf(p4.c_str(),"%d",&number);
    cout << "appname = "<< appname <<number << endl;
 
// start:
    char *seed,*pub;
    pub = (char *)pubkey;
    seed = VENDER_SEED;
//login & check : #if 1//  loginlic ,and checklic seprated  
    i = login_lic((char *)p1.c_str(),(char *)p2.c_str(),(char *)p3.c_str(),number,(char *)appname.c_str(),sign,sid);
    if (i<0) 
    {
        cout << "app license failed, please check  the  ELOG\n";
        exit(1);
    }
    cout << "sid return = "<<sid <<endl;
     
    if(!check_lic(sign,sid,pub,seed))
    {
        cout << "app checkValid failed\n";
        exit(1);
    }
 #else
    if(!login_check((char *)p1.c_str(),(char *)p2.c_str(),(char *)p3.c_str(),number,(char *)appname.c_str(),pub,seed))
    {
        cout << "app checkValid failed\n";
        exit(1);
    }
 #endif
    int delay; 
    sscanf(p5.c_str(),"%d",&delay);
//work: protected coeds:     
    sleep(delay); 
    cout << "p6 = " << p6.length() << p6;
//logout:
    if (p6.length() !=0) 
    {
        #if 0 // use packID:
        i = logout_lic((char *)p1.c_str(),(char *)p2.c_str(),(char *)p3.c_str());
        #else
        i = logout_licf();
        #endif
        if (i < 0) 
            cout << "app license logout  failed, please check  the  ELOG\n";
        else
             cout << "app   logout ok \n";

    }
    else
        cout << "app no logout\n";

    cout << "ok out";
}

    @endcode
  
    @b appMakefile:\n\n
  
    @code example apps.apro
  
 TETRIS_HOME = $(NGP)/libso/ncld
BOOST_HOME = $(NGP)/support/boost
JASON_HOME = $(NGP)/support/Jason
SSL_HOME = $(NGO)/support/ssl
LIC_ROOT_PATH = $NGP/licmng

LIBS_APP = \
	-L $${LIC_ROOT_PATH}/lib\
	-llicbase  -llicclient  -lliccommon  -llichttpcall -lcurl -llicconfig  /lib64/libdl.so.2  
LIBS_BUS = \
	-luuid\
	-L$${SSL_HOME} /lib -lcrypto \
	-L$${SSL_HOME}/ /lib -lssl \
	-L$${JASON_HOME} /lib -ljsoncpp\
	-L$${BOOST_HOME}/lib -lboost_date_time -lboost_filesystem -lboost_system -lboost_thread -lboost_serialization
  
TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = apps
INCLUDEPATH += .  $${LIC_ROOT_PATH}/file 
LIBS +=  $${LIBS_BUS}
LIBS +=  $${LIBS_APP} 
SOURCES += apps.cpp 

 *   @endcode
 */

