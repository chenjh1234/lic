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
    QString p1,p2,user;
    char ch[10];
    user = getenv("USER");
    if (user != ADMIN_USER) 
    {
        cout << "Error we have to shutdown Lic server as admin user:" << ADMIN_USER << endl;
        exit(1);
    }
    p1= "this command will shutdown licServer,Are you sure (y/n):";
    cout << p1.Q2CH ;
    cin >> ch;
    p2 = ch;
    if (p2 != "y") 
    {
        cout << "shutdown failed\n";
        exit(1);
    }
// start 
    int i;
    sc = new  StartClient();
    sc->client->shutDown();
     cout << "shutDown licServer ok\n";
    delete sc;

}

