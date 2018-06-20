#include <iostream>
using namespace std;
#include "startClient.h"
#include "testG.h"
#include "comDefs.h"
int should_exit = 0;
void sighandler(int /*signum*/)
{ // should_exit = 1;
   exit(1);
}
#include "appInclude.h"
main(int argc ,char **argv)
{
    cout <<"123\n";
 #if 1 
    signal (SIGINT, sighandler);
    StartClient *sc;
    QString p1,p2;
    QStringList slist;
    p1 ="";
    p2 ="";
    if (argc >=2)  p1 = argv[1];
    if (argc >=3)  p2 = argv[2];
    qDebug() << " p1,p2 = " << p1 <<p2;
  
// start
    sc = new  StartClient();
    slist = sc->client->report(p1,p2);
    int i;
    for (i = 0; i <slist.size();i++) 
    {
        cout << slist[i].Q2CH <<endl;
    }
    delete sc;
    #endif

}
