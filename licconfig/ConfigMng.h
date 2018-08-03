#ifndef CONFIG_MNG_H
#define CONFIG_MNG_H

 
#define CONFIG_REM_CHAR "#"
#define CONFIG_SEP_CHAR ":"
#define CONFIG1_SEP_CHAR ","
 
#define CONFIG_PACKID "package"

#include "LimitData.h"
#include "LimitConfig.h"

class ConfigMng
{
public:
    ConfigMng();
    ~ConfigMng();
// load file
    int loadFile(QString file);
    int loadStr(QString str);
    int readFile(QString file);
    int readStr(QString str);
    // decode:
    bool isPack(QString pack);// is packid exist;
    QStringList packages();
    int size();
    bool isNull();// if have any items, aoid to load a invalid file;
    LimitConfig * packLimit(QString pack);

// login:
    int login(QString packID,QString user,QString node,QString project,int n);
    int unlogin(QString packID,QString user,QString node,QString project,int n);
// db:
    QString dbFile();//{return CONFIG_DB_FILE;}
    void setDbFile(QString file);
  
    bool saveDB();
    bool loadDB();
// for reload file:
    int setRemove();// set status to STAT_REMOVE;, before reload file
    int clearRemoved();// after reload file;
    int clear();
// view
    QString view(QString pack = "" ,QString item="",QString name = "");    

 

private:
    int doWork(int ty);// setRemove;clearRemoved;
    int doDB(int ty,QDataStream &ds);
    QMap<QString,LimitConfig *> mapConfig;// for difrrent packages limitConfig
    int  readItem(int ty,QString text,QStringList packList,int limit);
    int readStream(QTextStream *in);
    QString _fileConfig;



};


#endif
