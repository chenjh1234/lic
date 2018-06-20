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
// unloadfile:
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
        cout <<"usage: unloadfile licfile proofFile\n";
        cout <<"licfile: is the license file that you want to unload\n";
        cout <<"proofFile: is the proof file output,you borrow Return with it\n";
        exit(1);
        }
   
     
// start 
    int i;
    sc = new  StartClient();
    i = sc->client->unloadFile(p1,p2);
    if (i < 0) 
    {
        cout << "unload file err, please check  the PLOG\n";
        delete sc;
        exit(1);
    }
    else
        cout << "unload file ok\n";
    delete sc;

}
