 
#ifndef EAST_LIC_H__ 
#define EAST_LIC_H__ 

#define east_login  east_login__
#define east_logout  east_logout__
#define east_logoutf  east_logoutf__
  
extern "C"
{
    int  east_login(char *pack,int &number,char *appname);//cannot be default in c
    int  east_logout(char *pack);
    int  east_logoutf();

}
#endif
