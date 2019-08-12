#include "LLicEncrypt.h"
#define MY_HELP  "createlic outfilename\n"\
"inputfile: licfile.sample \n"\
"    venderPri: 1: form licfile.sample, 2:\"vender.pri\"\n"\
"    venderseed: from licfile.sample  \n"\
"    vendername: from licfile.sample  \n"\
"    serverid: mid of the server (length = 35)  \n"\
"    servername: hostname or IP  of the server  \n"\
"    uuid: create new uuid automaticly  \n"\
"outputfile : arg1\n"\
"     if omit outfile = lic_clientname+_uuid_.lic\n"

#define INPUT_ERR "Error: input file  licfile.sample Parser error \n"
#define VENDERPRI_ERR "Error: vender Pri file is not exist!!!\n"
#define VENDERKEY_ERR "Error: get vender key error\n"
#define PACKAGEKEY_ERR "Error: get package key error\n"
#define WRITEFILE_ERR "Error: write license file  error\n"
#define MID_ERR "Error: mid length error\n"
//========================================start main=======================
#define PTYPE_TASK "task"
#define PTYPE_NODE "node"
#define PTYPE_USER "user"

void exitM(int id)
{
    char help[] = MY_HELP;
    cout << help;
    #ifndef TEST_UNIT
    exit(id);
    #else
    return;
    #endif

}
#ifndef TEST_UNIT
//#define SERVER_PUB "server.pub"
//#define SERVER_PRI "server.pri"
#define VENDER_PUB "vender.pub"
#define VENDER_PRI "vender.pri"

#define LIC_FILE_SAMPLE "licfile.sample"

main(int argc, char **argv)
{
#else
U_START(testCreateLic)
#endif
   
   int i;
   LLicEncrypt lic;
   LLicFile *lfile;
   QString str, fileo;
 //  LInfo * infoV,*infoP;
   LEncrypt en;
// open file:
   QString sPub, vPri, vSeed, venderKey, pKey;
   //i = lic.openLicFile(LIC_FILE_SAMPLE);//licfile.sample
   //i = lic.openLicFile(LIC_FILE); //licfile.sample
   fileo = "";
#ifndef TEST_UNIT
   if (argc >= 2) fileo = argv[1];
#endif

   lfile = new LLicFile();
   i = lfile->readFile((char *)LIC_FILE_SAMPLE);
   //qDebug() << "lfile.->readFile return = " << i;

#ifdef TEST_UNIT
   GT(i, 0);
#endif
   if (i <= 0)
   {
      cout << INPUT_ERR;
      exitM(1);
   }

   lic.setLicFile(lfile);
   i = lic.createKey();
#ifdef TEST_UNIT
   GT(i, 0);
#endif
   if(i <0)
   {
       qDebug() <<  lic._err ;
       exitM(1);
   }
   else
   {
       lfile->writeFile( fileo);
       delete lfile;
   }

#ifndef TEST_UNIT
}
#else
   U_END
#endif
