//#include "applic.h"
#include <string>
#include <fstream>
#include <stdio.h>  
#include <stdlib.h> 
#include <unistd.h> 
#include <vector> 
#include <iostream> 
//#include "appInclude.h"
using namespace std;
// the vender seed, publicKey;
#define VENDER_SEED    "0123456789" //
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


int loginlic(const char *vender,const char* pack,const char *version,int number,char* appname,char *sign,char *sid);
int logoutlic(const char *vender,const char* pack,const char *version);
int logoutlic();
bool checklic(char * sign,char *sid, char *pub,char *seed);
bool logincheck(const char *vender,const char* pack,const char *version,int number,char* appname,char *pub,char *seed);
// loadfile:
main(int argc ,char **argv)
{
    typedef vector<string> stringList;
    char sign[1024],sid[40];
    int i,number;
    // p1-6:vender,pacakge,version,number,delay,if logout
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
 
// start 
    char *seed,*pub;
    pub = (char *)pubkey;
    seed = VENDER_SEED;
#if 1 //  loginlic ,and checklic seprated
    i = loginlic(p1.c_str(),p2.c_str(),p3.c_str(),number,(char *)appname.c_str(),sign,sid);
    if (i<0) 
    {
        cout << "app license failed, please check  the  ELOG\n";
        exit(1);
    }
    cout << "sid return = "<<sid <<endl;
     
    if(!checklic(sign,sid,pub,seed))
    {
        cout << "app checkValid failed\n";
        exit(1);
    }
 #endif 
 #if 0
    if(!logincheck(p1.c_str(),p2.c_str(),p3.c_str(),number,(char *)appname.c_str(),pub,seed))
    {
        cout << "app checkValid failed\n";
        exit(1);
    }
 #endif
    int delay; 
    sscanf(p5.c_str(),"%d",&delay);
     
    sleep(delay); 
    cout << "p6 = " << p6.length() << p6;
    if (p6.length() !=0) 
    {
        #if 0 // use packID:
        i = logoutlic(p1.c_str(),p2.c_str(),p3.c_str());
        #endif
        i = logoutlic();
        if (i < 0) 
            cout << "app license logout  failed, please check  the  ELOG\n";
        else
             cout << "app   logout ok \n";

    }
    else
        cout << "app no logout\n";

    cout << "ok out";
}
