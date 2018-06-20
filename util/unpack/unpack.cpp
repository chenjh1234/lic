#include <iostream>
using namespace std;
#include "startClient.h"
#include "testG.h"
#include "comDefs.h"
#include "appInclude.h"
int should_exit = 0;
void sighandler(int /*signum*/)
{ // should_exit = 1;
   exit(1);
}
// unpack:
main(int argc ,char **argv)
{
    signal (SIGINT, sighandler);
    StartClient *sc;
    QString vender,pack,version,number,ty,uuid,pass;
    QStringList slist;
    
    if (argc >=8) 
    {
        vender = argv[1];
        pack = argv[2];
        version = argv[3];
        number = argv[4];
        ty = argv[5];
        uuid = argv[6];
        pass = argv[7];
    }
    else if (argc >=7) 
    {
        vender = argv[1];
        pack = argv[2];
        version = argv[3];
        number = argv[4];
        ty = argv[5];
        uuid = argv[6];
    }
    else if (argc >=6) 
    {
        vender = argv[1];
        pack = argv[2];
        version = argv[3];
        number = argv[4];
        ty = argv[5];
    }
    else if (argc >=5) 
    {
        vender = argv[1];
        pack = argv[2];
        version = argv[3];
        number = argv[4];
    }
        else
        {
        cout <<"usage: unpack vender pack version number type uuid pass\n";
        exit(1);
        }
   
     
// start 
    int i,n;
    QString str;
    n = number.toInt();

    sc = new  StartClient();
    if (pass == "")  
    {
        str = sc->client->unloadPackage(vender,pack,version,n,ty,uuid); 
    }
    else
    {
        str = sc->client->removePackage(vender,pack,version,n,ty,uuid,pass); 
    }
    i = str.length();
   

    if (i <= 0) 
    {
        cout << "unpack err, please check  the PLOG\n";
        delete sc;
        exit(1);
    }
    else
    {
        cout << str.Q2CH << endl;
        cout << "unpack ok\n";
    }      
        
    delete sc;

}
