
#ifndef APP_LIC_H__ 
#define APP_LIC_H__ 

int loginlic(const char *vender,const char* pack,const char *version,int number,char* appname,char *sign,char *sid);
int logoutlic(const char *vender,const char* pack,const char *version);
int logoutlic();
bool checklic(char * sign,char *sid, char *pub,char *seed);
bool logincheck(const char *vender,const char* pack,const char *version,int number,char* appname,char *pub,char *seed);
#endif


