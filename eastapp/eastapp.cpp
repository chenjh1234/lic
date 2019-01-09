#include "applic.h"
#include "east.h"
#include <string>
#include <fstream>
#include <stdio.h>  
#include <stdlib.h> 
#include <unistd.h> 
#include <vector> 
#include <iostream> 
//#include "appInclude.h"
using namespace std;
 
#if 0
int login_lic( char *vender,char* pack,char *version,int number,char* appname,char *sign,char *sid);
int logout_lic( char *vender,char* pack, char *version);
int logout_lic();
int logout_licf();
bool check_lic(char * sign,char *sid, char *pub,char *seed);
bool login_check(char *vender,char* pack,char *version,int number,char* appname,char *pub,char *seed);
#endif
 
main(int argc ,char **argv)
{
 
    int i,number;
    // p1-6:vender,pacakge,version,number,delay,if logout
    string p1,p2,p3,p4,appname;
//parameters:
    number = 0;
    p2 = "1";
    p3 = "100";
    p4 = "";
    if (argc <2)  
    {
        cout << "usage: eastapp packagename number delay logout\n";
        exit(1);
    }
    else 
    {
        p1 = argv[1];//pack
    }
    if (argc >2)  p2 = argv[2];//number
    if (argc >3)  p3 = argv[3];//delay
    if (argc >4)  p4 = argv[4];//logout


    
// start 
    char *seed,*pub,*pack,*app;
    appname = "eastapp";
    sscanf(p2.c_str(),"%d",&number);
    pack = (char *)p1.c_str();
    app = (char *)appname.c_str();
    cout << "appname ,number= "<< appname <<","<<number << endl;
  
    app="";
    i = east_login(pack,number,app);
     //   i = east_login(pack,number);
    cout << "ret eastlogin = " << i << endl;
    if (i<0) 
    {
        cout << "eastapp login failed \n";
        exit(1);
    }
 
//delay
    int delay; 
    sscanf(p3.c_str(),"%d",&delay);
     
    sleep(delay); 
    if (p4.length() !=0) 
    {
 
        i = east_logoutf();
        cout << "ret eastlogout = " << i << endl;
        if (i < 0) 
            cout << "eastapp   logout  failed\n";
        else
             cout << "eastapp   logout ok \n";

    }
    else
        cout << "app no logout\n";

    cout << "ok out";
}
