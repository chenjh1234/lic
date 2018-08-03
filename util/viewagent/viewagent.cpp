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

// start
    sc = new  StartClient();
    qDebug() << "after new";
    slist = sc->client->viewAgent(p1);
    int i;
    for (i = 0; i <slist.size();i++) 
    {
        cout << slist[i].Q2CH <<endl;
    }
    delete sc;

}

