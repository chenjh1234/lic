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
// borrow:
main(int argc ,char **argv)
{
    signal (SIGINT, sighandler);
    StartClient *sc;
    QString p1,p2;
    QStringList slist;
    p1 ="";
    if (argc >=3)  
    {
        p1 = argv[1];
        p2 = argv[2];
    }
        else
        {
        cout <<"usage: borrow licfile_un licfileOut\n";
        cout <<"licfile_un: is the updated license file that you want to borrow out\n";
        cout <<"licfileOut: is the output  license file that you will install in other server\n";
        cout <<"note:\n";
        cout <<"for licfile_un: you need to redefine :serverid,limit\n";
        cout <<"for licfile_un: limited to define one package,more than one package is not support\n";
        exit(1);
        }
  
     
// start 
    int i;
    sc = new  StartClient();
    i = sc->client->borrow(p1,p2);
    if (i < 0) 
    {
        cout << "borrow err, please check  the PLOG\n";
        delete sc;
        exit(1);
    }
    else
        cout << "borrow ok\n";
    delete sc;
}

