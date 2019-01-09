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
    QString p1,p2;
    p1 ="-1:-1:-1:-1";
    if (argc >=2)  p2 = argv[1];
    int dl;
    dl = 2;
    if (!p2.isEmpty()) 
    {
        dl = p2.toInt();
    }
// start 
    int i;
    sc = new  StartClient();
    sleep(dl);// wait for device connect
    p1 = sc->client->getDev(p1);
 
    cout << p1.Q2CH <<endl;

    delete sc;

}
