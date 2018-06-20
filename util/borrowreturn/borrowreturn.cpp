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
// borrowreturn:
main(int argc ,char **argv)
{
    signal (SIGINT, sighandler);
    StartClient *sc;
    QString p1;
    QStringList slist;
    p1 ="";
    if (argc >=2)  p1 = argv[1];
    else
    {
        cout <<"usage: borrowreturn proofFile\n";
        cout <<"proofFile: is the  file that when unload borrowOut licfile created\n";
        exit(1);
    }
     
// start 
    int i;
    sc = new  StartClient();
    i = sc->client->borrowReturn(p1);
    if (i < 0) 
    {
        cout << "borrow Return err, please check  the PLOG\n";
        exit(1);
    }
    else
        cout << "borrow Return ok\n";

    delete sc;

}

