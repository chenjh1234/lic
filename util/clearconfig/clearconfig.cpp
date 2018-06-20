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

main(int argc ,char **argv)
{
    signal (SIGINT, sighandler);
    StartClient *sc;
    QString p1;
    QStringList slist;
    p1 ="";
    if (argc >=2)  p1 = argv[1];
    qDebug() << " p1 = " << p1 ;
    int i;
// start
    sc = new  StartClient();
    i = sc->client->clearConfig(p1);
    delete sc;
    if (i <0) 
    {
        cout << "clearconfig error \n";
        exit(1);
    }
     else 
    {
            cout << "clearconfig ok \n";
            exit(0);
    }
 
}

