#include <iostream>
#include "configMng.h"
#include "testG.h"
#include <QThread>
#define SET_LOGIN 0
#define SET_UNLOGIN 1
ConfigMng cf;
class TH:public QThread
{
public:
    TH(){QThread();};
    void run()
    {
        if (flag == SET_LOGIN)  
            cf.login(pack, user, node, project, 1);
        else
            cf.unlogin(pack, user, node, project, 1);

    }
    void setName(QString p,QString u,QString n,QString pr,int f)
    {
        pack = p;
        user = u;
        node = n;
        project = pr;
        flag = f;
    };
    QString pack,user,node,project;
    int flag;

};

using namespace std;


U_START(loadfile)
#if 0
package: east,east1 
user:cjh,djg:5
user:sxp:10
node:pc1:20
project:myp:30

# second pack
package: east2
user:sxp:5
user:*:10
#endif
    int i,sz,n;
    QString file,name;
    QStringList slist,Slist;
    file = "lic.conf";
    i =  cf.loadFile(file);
    EQ(i,0);
    i= cf.size();
    EQ(i,3);
 
    slist = cf.packages();   
    qDebug() << "packs = " << slist;
    i=0; //first package
    //user:
        sz = cf.packLimit(slist[i])->sizeUser();
        qDebug() << "sz user";
        Slist = cf.packLimit(slist[i])->configUser.keys();
        qDebug() << "users = " << Slist;
        name = "sxp";
        n = 10;
        EQ(cf.packLimit(slist[i])->configUser[name]->limit(),n)

        sz = cf.packLimit(slist[i])->sizeNode();
        Slist = cf.packLimit(slist[i])->configNode.keys();
        qDebug() << "nodes = " << Slist;
        name = "pc1";
        n = 20;
        EQ(cf.packLimit(slist[i])->configNode[name]->limit(),n);

        sz = cf.packLimit(slist[i])->sizeProject();
        Slist = cf.packLimit(slist[i])->configProject.keys();
        qDebug() << "projects = " << Slist;
        name = "myp";
        n = 30;
        EQ(cf.packLimit(slist[i])->configProject[name]->limit(),n);
     i=1; //2rd package
    //user:
        sz = cf.packLimit(slist[i])->sizeUser();
        Slist = cf.packLimit(slist[i])->configUser.keys();
        qDebug() << "users = " << Slist;
        name = "sxp";
        n = 10;
        EQ(cf.packLimit(slist[i])->configUser[name]->limit(),n)

        sz = cf.packLimit(slist[i])->sizeNode();
        Slist = cf.packLimit(slist[i])->configNode.keys();
        qDebug() << "nodes = " << Slist;
        name = "pc1";
        n = 20;
        EQ(cf.packLimit(slist[i])->configNode[name]->limit(),n);

        sz = cf.packLimit(slist[i])->sizeProject();
        Slist = cf.packLimit(slist[i])->configProject.keys();
        qDebug() << "projects = " << Slist;
        name = "myp";
        n = 30;
        EQ(cf.packLimit(slist[i])->configProject[name]->limit(),n);
        i=2; //3d package
    //user:
        sz = cf.packLimit(slist[i])->sizeUser();
        Slist = cf.packLimit(slist[i])->configUser.keys();
        qDebug() << "users = " << Slist;
        name = "sxp";
        n = 5;
        EQ(cf.packLimit(slist[i])->configUser[name]->limit(),n)
        name = "*";
        n = 10;
        EQ(cf.packLimit(slist[i])->configUser[name]->limit(),n)
        QString str;
        str = cf.view();
        qDebug() << str;
        str = cf.view("east");
        qDebug() << str;
        str = cf.view("east1","*","cjh");
        qDebug() << str;
        str = cf.view("east2","*","cjh");
        qDebug() << str;
    U_END
U_START(reload)
    #if 0
package: east 
user:cjh:10
node:pc1:30
project:pc2:30
 
# second pack
package: east2
user:sxp:10
node:*:20
project:pp:30
    #endif
        int i,n,sz;
    QString file,name,fileadd;
    QStringList slist,Slist;
    //file = "lic_add.conf";
    fileadd = "lic_add.conf";
    qDebug() << cf.view();
   // i = cf.setRemove();
   // PR("after setRemove\n");
    #if 0 //  we test clear all
    qDebug() << cf.view();
    i = cf.clearRemoved();
    PR("after clearRemoved\n");
    qDebug() << cf.view();
    EQ(i,0);
    i= cf.size();
    EQ(i,0);
    qDebug() <<"size=" <<  i;
    #endif
#if 1 // we test load other file:
    i =  cf.loadFile(fileadd);

    EQ(i,0);
    qDebug() << cf.view();

  //  i = cf.clearRemoved();
    EQ(i,0);
    i= cf.size();
    EQ(i,2);
    qDebug() << "size = " << i;
    qDebug() << cf.view();
#endif
    slist = cf.packages();   
    qDebug() << "packs = " << slist;
    i=0; //first package
    //user:
        sz = cf.packLimit(slist[i])->sizeUser();
        Slist = cf.packLimit(slist[i])->configUser.keys();
        qDebug() << "users = " << Slist;
        name = "cjh";
        n = 10;
        EQ(cf.packLimit(slist[i])->configUser[name]->limit(),n)

        sz = cf.packLimit(slist[i])->sizeNode();
        Slist = cf.packLimit(slist[i])->configNode.keys();
        qDebug() << "nodes = " << Slist;
        name = "pc1";
        n = 30;
        EQ(cf.packLimit(slist[i])->configNode[name]->limit(),n);

        sz = cf.packLimit(slist[i])->sizeProject();
        Slist = cf.packLimit(slist[i])->configProject.keys();
        qDebug() << "projects = " << Slist;
        name = "pc2";
        n = 30;
        EQ(cf.packLimit(slist[i])->configProject[name]->limit(),n);
     i=1; //2rd package
    //user:
        sz = cf.packLimit(slist[i])->sizeUser();
        Slist = cf.packLimit(slist[i])->configUser.keys();
        qDebug() << "users = " << Slist;
        name = "sxp";
        n = 10;
        EQ(cf.packLimit(slist[i])->configUser[name]->limit(),n)

        sz = cf.packLimit(slist[i])->sizeNode();
        Slist = cf.packLimit(slist[i])->configNode.keys();
        qDebug() << "nodes = " << Slist;
        name = "*";
        n = 20;
        EQ(cf.packLimit(slist[i])->configNode[name]->limit(),n);

        sz = cf.packLimit(slist[i])->sizeProject();
        Slist = cf.packLimit(slist[i])->configProject.keys();
        qDebug() << "projects = " << Slist;
        name = "pp";
        n = 30;
        EQ(cf.packLimit(slist[i])->configProject[name]->limit(),n);

    U_END

U_START(login)
    #if 0
package: east 
user:cjh:10
node:pc1:30
project:pc2:30
 
# second pack
package: east2
user:sxp:10
node:*:20
project:pp:30
    #endif
        int i,n,sz,ir;
    QString pack,user,node,config,project;
    QStringList slist,Slist;

    slist = cf.packages();   
    pack = "east";
    user = "cjh";
    node = "pc1";
    project = "pc2";
    n = 2;
 //east:
    ir = cf.login(pack,user,node,project,n);
    EQ(ir,LOGIN_OK);
   
        EQ(cf.packLimit(pack)->configUser[user]->count(),n);
        EQ(cf.packLimit(pack)->configNode[node]->count(),n);
        EQ(cf.packLimit(pack)->configProject[project]->count(),n);
    n = 1;
    ir = cf.unlogin(pack,user,node,project,n);
        EQ(ir,LOGIN_OK);
       
            EQ(cf.packLimit(pack)->configUser[user]->count(),n);
            EQ(cf.packLimit(pack)->configNode[node]->count(),n);
            EQ(cf.packLimit(pack)->configProject[project]->count(),n);
//east:
    node = "pc31";
    project = "pc32";
    ir = cf.login(pack,user,node,project,n);
    qDebug() << "login = " << ir;
    EQ(ir,LOGIN_OK);
   
        EQ(cf.packLimit(pack)->configUser[user]->count(),2);
        //EQ(cf.packLimit(pack)->configNode[node]->count(),0);
        //EQ(cf.packLimit(pack)->configProject[project]->count(),0);
    n = 2;
    ir = cf.unlogin(pack,user,node,project,n);
        EQ(ir,LOGIN_OK);
       
            EQ(cf.packLimit(pack)->configUser[user]->count(),0);
           // EQ(cf.packLimit(pack)->configNode[node]->count(),0);
           // EQ(cf.packLimit(pack)->configProject[project]->count(),0);
// east2:
    pack="east2";
    user = "sxp";
    node = "pc1";
    project = "pp";
    n = 5;
   qDebug() << " BG LOGIN east2";
    ir = cf.login(pack,user,node,project,n);

qDebug() << "login:=====" << cf.view(pack);
    EQ(ir,LOGIN_OK);
        EQ(cf.packLimit(pack)->configUser[user]->count(),n);
        EQ(cf.packLimit(pack)->configNode[node]->count(),n);
        EQ(cf.packLimit(pack)->configProject[project]->count(),n);
    n =3;
 
    ir = cf.unlogin(pack,user,node,project,n);
        EQ(ir,LOGIN_OK);
qDebug() << "unlogin:=====" << cf.view(pack);
        
        ir = 2;
            EQ(cf.packLimit(pack)->configUser[user]->count(),ir);
            EQ(cf.packLimit(pack)->configNode[node]->count(),ir);
            EQ(cf.packLimit(pack)->configProject[project]->count(),ir);

qDebug() << "login err :=====" << cf.view(pack);
    n = 10;
    ir = cf.login(pack,user,node,project,n);
    EQ(ir,LOGIN_FAILED)
qDebug() << "login full :=====" << cf.view(pack);
    n = 8;
    ir = cf.login(pack,user,node,project,n);
    EQ(ir,LOGIN_OK);

    qDebug() << cf.view(pack);

    U_END

U_START(db)
    
    int i,n,sz,ir;
    QString pack,user,node,config,project;
    QStringList slist,Slist;

    slist = cf.packages();   
    pack="east2";
    user = "sxp";
    node = "pc1";
    project = "pp";
    n = 5;

    ir = cf.saveDB();
       qDebug() << cf.view();
    REM(saveDB)
    EQ(ir,DSTAT_OK);

    ir = cf.setRemove();
     REM(setRemove)
     EQ(ir,DSTAT_OK);

    ir = cf.clearRemoved();
    REM(clearRemoved)
     EQ(ir,DSTAT_OK);
    qDebug() << cf.view();

    ir = cf.loadDB();
     REM(loadDB)
    EQ(ir,DSTAT_OK);
    qDebug() << cf.view();
           ir = 10;
            EQ(cf.packLimit(pack)->configUser[user]->count(),ir);
            EQ(cf.packLimit(pack)->configNode[node]->count(),ir);
            EQ(cf.packLimit(pack)->configProject[project]->count(),ir);

U_END;

U_START(lock)
    int i,n,sz,ir,n1,nn;
    QString pack,user,node,config,project,file;
    QStringList slist,Slist;
    cf.clear();
    file = "lic100.conf";
    ir = cf.loadFile(file);
    EQ(ir,DSTAT_OK);

    slist = cf.packages();   
    pack="pr100";
    user = "cjh";
    node = "pc";
    n = 200;
    n1 = 200;
    nn = 10;
    TH th[2000];

    for (i = 0; i <n;i++) 
    {
        project = QString("%1").arg(i/nn);
        th[i].setName(pack,user,node,project,SET_LOGIN);
        th[i].start();
      
    }
     for (i = 0; i <n ;i++) 
    {
        th[i].wait();
    }
     qDebug() << cf.view(pack);
    EQ(cf.packLimit(pack)->configUser[user]->count(),n);
    EQ(cf.packLimit(pack)->configNode[node]->count(),n);
    for (i = 0; i <n/nn;i++) 
    {
         project =  QString("%1").arg(i);
         EQ(cf.packLimit(pack)->configProject[project]->count(),nn);
    }
    qDebug() << "login ok------------------------------";
 
    int ii;
     for (ii = 0; ii <n1;ii++) 
    {
          i=n+ii;
        project =  QString("%1").arg(ii / nn);
        th[i].setName(pack,user,node,project,SET_UNLOGIN);
        th[i].start();
      
    }

    for (ii = 0; ii <n1;ii++) 
    {
        i=n+ii;
        th[i].wait();
    }
    qDebug() << "unlogin out  ok------------------------------";

    EQ(cf.packLimit(pack)->configUser[user]->count(),0);
    EQ(cf.packLimit(pack)->configNode[node]->count(),0);
    for (i = 0; i <n/nn;i++) 
    {
         project =  QString("%1").arg(i);
         EQ(cf.packLimit(pack)->configProject[project]->count(),0);
    }

    qDebug() << cf.view(pack);

      

U_END
U_START(loadnull)
    int i;
    QString fileNull;
   
    i = cf.loadFile("lic.conf");
    EQ(i,0);
    qDebug() << cf.view();
    fileNull = "lic_null.conf";
    i = cf.loadFile(fileNull);
    qDebug() << "load null file ret =  " << i;
    LT(i,0);
    qDebug() << cf.view();
    i= cf.size();
    EQ(i,3);



U_END

//int main (int argc, char *argv[])
//{
M_START
    cf.setDbFile("dbConfig.db");
    U_TEST(loadfile)// load a file: lic.conf
    U_TEST(reload) // setRemove,load other file(lic_add.conf) ,clearRemoved: left the items in new file;
    U_TEST(login)//  login and unlogin: include full, and failed caused by apply resource number more than limit;
    U_TEST(db);// save db,load db,;ok
    U_TEST(lock) //lock: multithread access the config data;
    U_TEST(loadnull)
M_END
//}

