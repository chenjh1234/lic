#if 0
#include <iostream>

using namespace std;

int main (int argc, char *argv[])
{
    cout << "hello world" << endl;
    return(0);
}
#endif

#include <iostream>
#include <QString>
#include <stdio.h>
#include "comDefs.h"
using namespace std;
int startServer(int argc, char *argv[]);
int main (int argc, char *argv[])
{
   // cout << "hello world" << endl;
    QString user;
  #if 0
    user = getenv("USER");
    if (user != ADMIN_USER) 
    {
        cout << "Error we have to shutdown Lic server as admin user : " << ADMIN_USER << endl;
        exit(1);
    }
    #endif
    startServer(argc, argv); 
    return(0);
}
