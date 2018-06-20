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
// loadfile:
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
        cout <<"usage: loadfile licfile\n";
        cout <<"licfile: is the license file that vender supplied\n";
        exit(1);
    }
     
// start 
    int i;
    sc = new  StartClient();
    i = sc->client->loadFile(p1);
    if (i < 0) 
    {
        cout << "load file err, please check  the PLOG\n";
        delete sc;
        exit(1);
    }
    else
        cout << "load file ok\n";

    delete sc;

}
