#ifndef LIMIT_CONFIG_H
#define LIMIT_CONFIG_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QDebug>


#include "LimitData.h"
#define CONFIG_ALL_CHAR "*"
 
#define ITEM_USER 0
#define ITEM_NODE 1
#define ITEM_PROJECT 2

#define CONFIG_USER "user"
#define CONFIG_NODE "node"
#define CONFIG_PROJECT "project"

#define ALL_CLEAR -1

#define ERR_LOGIN_USER -2
#define ERR_LOGIN_NODE -3
#define ERR_LOGIN_PROJECT -4

#define DO_SET_REMOVE 0
#define DO_CLEAR_REMOVED 1
#define DO_SAVEDB 2
#define DO_LOADDB 3
#define DO_ISNULL 4

#define DO_LOGIN 0
#define DO_UNLOGIN 1
#define DO_CANLOGIN 2




#define TAB_0 QString("     ")
#define TAB_1 TAB_0 + "     "
#define TAB_2 TAB_1 + "     "


class LimitConfig
{
public:

    LimitConfig();
    ~LimitConfig();
// file decode used:   
    int setUserLimit(QString name,int n);
    int setNodeLimit(QString name,int n);
    int setProjectLimit(QString name,int n);

    bool isUser(QString);
    bool isNode(QString);
    bool isProject(QString);

// manage limit used: 0:ok,-1:user,-2:node err;-3:project err;
    int login(QString user,QString node,QString project,int n);
    int unlogin(QString user,QString node,QString project,int n);
    bool canLogin(QString user,QString node,QString project,int n);

    bool isUserAll();
    bool isNodeAll();
    bool isProjectAll();
    bool isAll( QMap<QString,LimitData *> &who);

    bool isNull();
// reload file:
    int setRemove();// set status to STAT_REMOVE;
    int clearRemoved();
// db:
    int saveDB(QDataStream &ds);
    int loadDB(QDataStream &ds);
    int doDB(int ty,QMap<QString,LimitData *> &who,QDataStream &ds);

//decode:
    bool isValid(int ty,QString name);
    int setLimit(int ty,QString name,int n);// processing All "*" option
    // for ALL option:
    int setLimitAll(int ty,int n);
    int setUserAll(int n);
    int setNodeAll(int n);
    int setProjectAll(int n);
    int clearUserAll();
    int clearNodeAll();
    int clearProjectAll();

    int getAll(QMap<QString,LimitData *> &who);

    
    int insert(int ty,QString name,LimitData *ldata);
    int sizeUser(){return configUser.size();};
    int sizeNode(){return configNode.size();};
    int sizeProject(){return configProject.size();};

    QString  view(QString item="" ,QString name=""  );
    QString  viewItem(QMap<QString,LimitData *> &who ,QString name );

    QMap<QString,LimitData *> configUser,configNode,configProject;
    int userLimit,nodeLimit,projectLimit;// for ALL options

    
    // do the ALL situation:
private:
     int doRemove(int ty);//ty:setRemove;cearRemoved
     int doRemove(int ty,QMap<QString,LimitData *> & who);
     int doLogin(int ty,QMap<QString,LimitData *> & who,QString name,int n);
     QMutex _lock;

};
 

#endif
