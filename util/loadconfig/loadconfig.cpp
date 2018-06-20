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
    p1 ="";
    if (argc >=2)  p1 = argv[1];
    else
    {
        cout <<"usage: loadconfig licfile\n";
        cout <<"licfile: is the config file \n";
        exit(1);
    }
     
// start 
    int i;
    sc = new  StartClient();
    i = sc->client->loadConfig(p1);
    if (i < 0) 
    {
        cout << "load config file err, please check  the PLOG\n";
        delete sc;
        exit(1);
    }
    else
        cout << "load config file ok\n";

    delete sc;

}
