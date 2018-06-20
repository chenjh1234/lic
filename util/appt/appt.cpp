#include "appClient.h"
#include "appInclude.h"
#define VENDER_SEED    "0123456789"
      unsigned char pubkey[] = \
      "-----BEGIN PUBLIC KEY-----\n"\
      "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDLpkCvoO/bC7bY2iAKK/LX+yMK\n"\
      "CnL6FpatrM9Yk59ce7/S/T7OqgRhZYMIhGU6rJOB7finilEYDAtt5OJYUqNF+cS0\n"\
      "vZENDDsWWPvUYcYEcUurXN/G0PbcTjU90mqcNXx5GC9AQQeV3PYiqzHhYoJqKECB\n"\
      "ibzelvxMWGgLUZM+swIDAQAB\n"\
      "-----END PUBLIC KEY-----\n";

unsigned char help[] = \
"usage: appt vender package version number time logout\n"\
"p1:vender: vender name\n"\
"p2:package: package name\n"\
"p3:version: version name\n"\
"p4:number number license apply(1)\n"\
"p5:time in second the app spend(100)\n"\
"p6:if present: we do logout\n";


// loadfile:
main(int argc ,char **argv)
{
    
    

    int i;
    QString p0,p1,p2,p3,p4,p5,p6,appname;
    QStringList slist;
    p4 ="1";
    p5 ="100";
    p6 ="";
//parameters:
    if (argc <4)  
    {
        cout << help;
        exit(1);
    }
    else 
    {
        p0 = argv[0];
        p1 = argv[1];
        p2 = argv[2];
        p3 = argv[3];
    }
    if (argc >=5)  p4 = argv[4];
    if (argc >=6)  p5 = argv[5];
    if (argc >=7)  p6 = argv[6];

    slist << p0<<p1<<p2<<p3<<p4<<p5<<p6;
    appname = "";
    for (i = 0; i <7; i++) 
    {
        if (i !=6)  
            appname = appname +slist[i]+ "_"; 
        else
            appname = appname + slist[i];
    }

    qDebug() << "appname = "<< appname;

    AppClient app;
// start 
    slist = app.login(p1,p2,p3,p4.toInt(),appname);
    if (slist.size() < 2) 
    {
        cout << "app license failed, please check  the  ELOG\n";
        exit(1);
    }
    QString seed,pub;
    char *ch;
    ch = (char *)pubkey;
    pub = QString(ch);
    seed = VENDER_SEED;
    if (! app.checkValid(slist,pub, seed)) 
    {
        cout << "app checkValid failed\n";
        exit(1);
    }
    int delay; 
    delay = p5.toInt();
    qDebug() << "delay = " << delay;
    
    sleep(delay); 
    qDebug() << "p6 = " << p6.length() << p6;
    if (p6.length() !=0) 
    {
        i = app.logout(p1,p2,p3);

        if (i < 0) 
            cout << "app license logout  failed, please check  the  ELOG\n";
        else
             qDebug() << "app   logout ok ";

    }
    else
        qDebug() << "app no logout";

    qDebug() << "ok out";
 
}
