

#include "SLicMng.h"
main(int argc,char **argv)
{
    SLicMng mng;
    int para;
    para = 0;
    // 0: showDB
    // 1:showPackDB;
    if (argc >=2) para = 1;
    mng.data->showDB(para); 
}
