#include <iostream>
using namespace std;
#include "startClient.h"
#include "testG.h"
#include "appInclude.h"
int should_exit = 0;
void sighandler(int /*signum*/)
{ // should_exit = 1;
   exit(1);
}

// a test from client for all APIs//
// lictest : it must be run in licwork dir or in files dir if it is installed

StartClient *sc;
#define FILES \
    QString file,file1,file2,file3,str,pfile,borrowIn,borrowOut,fileUser10; \
    file = "../licwork/test/test_lic_task2.lic"; \
    borrowIn = "../licwork/test/test_borrowInUser.lic";\
    borrowOut = "../licwork/test/test_borrowOutUser.lic";\
    pfile = "../licwork/test/test_borrowOutUser.lic.proof";\
    file1 = "../licwork/test/test_lic_node.lic";\
    file2 = "../licwork/test/test_lic_user.lic";\
    fileUser10 = "../licwork/test/test_lic_user10.lic";\
//report
void report()
{
   FILES;
   int sz, i;
   QStringList slist;
   slist = sc->client->report();
   sz = slist.size();
   GT(sz, 0)
   PR("size = " + STR(sz));
   for (i = 0; i < sz; i++)
   {
      PR(slist[i].Q2CH);
   }
}
//======================================
U_START(loadfile)

int i,sz;
FILES
// load file:
if (sc != NULL)  i = sc->client->loadFile(file);
qDebug()<< "ret = "<< i << i;
GT(i, 0);
if (sc != NULL)  i = sc->client->loadFile(fileUser10);
qDebug()<< "ret = "<< i << i;
GT(i, 0);
//report
report();
QStringList slist;
// pacakge
slist = sc->client->report(REPORT_PARAM_PACKAGE,"geoeast_pc1_1.0");
sz = slist.size();
EQ(sz, 1)
PR("size = " +STR(sz));
for (i = 0; i < sz; i++)
{
   PR(slist[i].Q2CH);
}

U_END
//======================================
U_START(loadBorrow)

int i,sz;
FILES
   bool b;
// load borrow:
if (sc != NULL)
{
   //b = sc->client->setCheckUUID(false);
   //EQ(b, true);
   i = sc->client->loadFile(borrowOut);

   //b = sc->client->setCheckUUID(true);
   //EQ(b, true);
}
qDebug()<< "ret = "<< i << i;
GT(i, 0);
//report
report();
U_END
//======================================
U_START(loginapp)


int i,number,sz;
QString vender,pack,version,app,pack1;
QStringList slist;

vender = "geoeast";
pack = "pc1";
pack1 = "pc11";
version = "1.0";
number = 2;
app = "myapp";

// pacakge
slist = sc->client->loginApp(vender,pack,version,number,app);

sz = slist.size();
EQ(sz, 2)

slist = sc->client->loginApp(vender,pack1,version,number,app);
sz = slist.size();
EQ(sz, 0)

//report

slist = sc->client->report(REPORT_PARAM_APP);
sz = slist.size();
EQ(sz, 1)
PR("app size = " +STR(sz));
for (i = 0; i < sz; i++)
{
   PR(slist[i].Q2CH);
}
// logout:
i = sc->client->logoutApp(vender,pack,version);
GT(i, 0)

//report

slist = sc->client->report(REPORT_PARAM_APP);
sz = slist.size();
EQ(sz, 0)
PR("app size = " +STR(sz));
for (i = 0; i < sz; i++)
{
   PR(slist[i].Q2CH);
}
U_END
//======================================
U_START(borrow)
int i,sz;
FILES
   i = -1;
// borrow  :
if (sc != NULL)  i = sc->client->borrow(borrowIn, borrowOut);
qDebug()<< "ret = "<< i << i;
GT(i, 0);
//report
report();
U_END
//int startC(int argc, char *argv[]);
//int main (int argc, char *argv[])
//======================================
U_START(unloadfile)
int i,sz;
FILES
   i = -1;
// unload file:
if (sc != NULL)  i = sc->client->unloadFile(borrowOut, pfile);
qDebug()<< "ret = "<< i << i;
GT(i, 0);
report();
U_END
//======================================
U_START(borrowReturn)
int i,sz;
FILES
   i = -1;
// unload file:
if (sc != NULL)  i = sc->client->borrowReturn(pfile);
qDebug()<< "ret = "<< i << i;
GT(i, 0);
//report
report();
U_END
//======================================
//======================================
U_START(unloadPackage)
int i,sz;
FILES
   i = -1;
// unload file:
if (sc != NULL)  
    str = sc->client->unloadPackage("geoeast","pc11","1.0",2);
qDebug()<< "ret = "<< str.length() << str;
EQ(str.length(), 0);
report();

if (sc != NULL)  
    str = sc->client->unloadPackage("geoeast","pc11","1.0",10);
qDebug()<< "ret = "<< str.length() << str;
GT(str.length(), 0);
//report
report();
U_END
 

M_START
#if 1
    // cout << "hello world" << endl;
    signal (SIGINT, sighandler);

    //startC(argc,argv);
    sc = new  StartClient();
    /* wait for the 'Ctrl+C' */
    U_TEST(loadfile)
    // U_TEST(loginapp)
    U_TEST(borrow)
    U_TEST(loadBorrow)
    U_TEST(unloadfile)
    U_TEST(borrowReturn)
    U_TEST(unloadPackage)

    //while (!should_exit)
    //		sleep (1);
    delete sc;
    //return(0);
    #endif


M_END

