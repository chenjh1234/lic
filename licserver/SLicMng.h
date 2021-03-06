#ifndef SLIC_MNG_H
#define SLIC_MNG_H


#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include <QMap>

//#define CHECK_UUID 0

#include "SLicData.h"
#define OK 0
#define LOADFILE_FILE_ERR -1
#define LOADFILE_VENDERKEY_ERR -2
#define LOADFILE_PACKAGEKEY_ERR -3
#define LOADFILE_MID_ERR -4
#define LOADFILE_VENDERSIGN_ERR -5
#define LOADFILE_UUID_ERR -6
#define LOADFILE_LOAD_ERR -7
#define LOADFILE_DATE_ERR -8

#define CONFIG_LIMIT_ERR -10

#define LOAD_LIC_FILE 0
#define LOAD_LIC_STR 1

#define INPUT_ERR "Error: input file  licfile.sample Parser error \n"
#define SERVERPUB_ERR "Error: server Pub file is not exist!!!\n"
#define VENDERPRI_ERR "Error: vender Pri file is not exist!!!\n"
#define VENDERKEY_ERR "Error: get vender key error\n"
#define PACKAGEKEY_ERR "Error: get package key error\n"
#define WRITEFILE_ERR "Error: write license file  error\n"
#define MID_ERR "Error: mid length error\n"
#define MID_ERR "Error: mid length error\n"

#define LOADFILE_ERR { \
    (char *)"Load file: openfile ok",\
    (char *)"Load file: openfile  error!!",\
    (char *)"Load file: verify venderKey error!!",\
    (char *)"Load file: verify packageKey error!!",\
    (char *)"Load file: MID error!!",\
    (char *)"Load file: venderSign error!!",\
    (char *)"Load file: uuid error,the licFile already loaded!!",\
    (char *)"Load file: load error,add to database error!!",\
    (char *)"Load file: date error",\
    NULL}

class SLicMng
{
public:
    SLicMng();
    ~SLicMng();
   
//load file:unload file
    //int loadFile(QString filename,QString serverPri);
   // int checkLicFile(QString filename,QString serverPri);// loadFile used
     int loadFile(QString filename);
     int checkLicFile(QString filename);// loadFile used

     int loadFileStr(QString str);
     int checkLicStr(QString str);// loadFile used
     int checkLic(QString str,int mode);// loadFile used
     int loadLic(QString str,int mode);// loadFile used

    //
    //int unloadPackage(QString vender,QString package,QString version,QString serverPri);
    //int unloadFile(QString filename,QString serverPri);

    int unloadFile(QString filename,QString &proofStr);
    int unloadFileStr(QString str,QString &proofStr);
    int unloadLic(QString str,QString &proofStr,int mode );
    QString unloadPackage(QString vender,QString pack,QString version,QString number="",QString ty="",QString uuid="",QString pass="");

    int removeFile(QString filename);
    int removeFileStr(QString filename);
    int removeFile(QString filename,int mode);

//app login logout
    int loginApp(SAppInfo &msg);
    bool isLoginConfig(SAppInfo &msg);
    bool isLogoutConfig(SAppInfo &msg);
    int clearConfig(QString passwd);

    int logoutApp(SAppInfo &msg);
    QString  getAppID(SAppInfo &msg);
    QString  getPackID(SAppInfo &msg);
   
//app heart beat:
    int heartBeat(SAppInfo &msg);// busy used
// borrow
    //QString filename, QString sPubIn,QString sPubOut,QString fo
    //int borrow(QString vender,QString package,QString version,QString type ,int number,QString start,QString end, QString mid);
    //int borrow(QString filename, QString sPUBIn,QString sPUBOut,QString fo);
    int borrow(QString &filename,QString &fo,int mode);//mode:0:file,1:string
    int borrowFileStr(QString &str,QString &stro);//  input is lic string
    int borrowFile(QString &filename,QString &fout);// input is filename,

    int borrowReturn(QString Proof);// proof is a number of string,sperated by \n:each str is encrypted packageInfo and hex encoded.
// dbpack2DB:
    int packDB2DB();
// report:
    QStringList reportPackage(QString packid);
    QStringList reportPackage();
    QStringList reportApp(QString packid);
    QStringList reportApp();

    //SLicData * getData(){return _data;};
    //void setData(SLicData* dt){_data = dt;};
// check:
    void setCheckUUID(bool b);
    bool isCheckUUID();
    bool isUuid(QString packid,QString uuid);  
    bool isVenderSign(QString vendername,QString venderSign);
    // encode:

    bool startup();
    void down();
    // lstjob:
     
    static SLicData * data;
//private:
   
    void init();
    QMap<int,QString>  mapLoadFile;// for load file error;

};


#endif
