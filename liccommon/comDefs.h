#ifndef COM_DEFS_H__ 
#define COM_DEFS_H__ 
#define CLIENT_LIB_EXPORT Q_DECL_EXPORT
#define SERVER_LIB_EXPORT Q_DECL_EXPORT
//#include <json/json.h>
#define CA_CERT "/s0/u/cjh/tetris/src/third/ca/cacert.pem"
#define CA_KEY  "/s0/u/cjh/tetris/src/third/ca/cakey.pem"

#define CA_CERT "/home/cjh/tetris/src/third/ca/cacert.pem"
#define CA_KEY  "/home/cjh/tetris/src/third/ca/cakey.pem"

#define CA_CERT "/home/cjh/ca/cacert.pem"
#define CA_KEY  "/home/cjh/ca/cakey.pem"
//env:
//LIC_ROOT_PATH
// env for client:
#define ENV_LIC_AGENT "LIC_AGENT" // is the agent device ID "5:0:1:0" is is not NULL will login to agent device
#define ENV_PORTAL_IP "PORTAL_IP"   // if exist Noserver will connect portal
#define ENV_PORTAL_PORT "PORTAL_PORT"
#define ENV_LIC_PROJECT "LIC_PROJECT"
#define PORTAL_IP "localhost"
#define PORTAL_PORT "18080"
#define PORTAL 1 
#define AGENT_PORT 38080
// portal:
#define CALL_NOTIFY "notify" // tell potal to start notify;
#define LIC_URL "lm"
#define LICA_URL "lma"
//command:
#define CMD_LOADFILE "loadFile"
#define CMD_UNLOADFILE "unloadFile"
#define CMD_UNLOADPACKAGE "unloadPackage"
#define CMD_REMOVEPACKAGE "removePackage"
#define CMD_LOGINAPP "loginApp"
#define CMD_LOGOUTAPP "logoutApp"
#define CMD_HEARTBEAT "heartBeat"
#define CMD_BORROW "borrow"
#define CMD_BORROWRETURN "borrowReturn"
#define CMD_REPORT "report"
#define CMD_SHUTDOWN "shutDown"
#define CMD_LOADCONFIG "loadConfig"
#define CMD_VIEWCONFIG "viewConfig"
#define CMD_CLEARCONFIG "clearConfig"
//#define CMD_GETDEV "getDev"

#define CMD_VIEWAGENT "viewAgent"

#define CMD_TIMEOUT boost::posix_time::time_duration (0,0,30)

//#define CMD_CHECKUUID "checkUUID"
#define CMD_HELLO "hello"

#define CMD_NUMBERS 15
#define CMD_AGENT 9

#define LIC_SERVER_ID  "5:0:0:0"
#define LIC_AGENT_ID  "5:0:1:0"
#define LIC_UM_ID  "1:0:0:0"

//load lic:
#define CMD_PARAM "cmd_param" 
 
// report:
#define REPORT_PARAM "report_param"
#define REPORT_PARAM_SINGLE "report_param_single"

#define REPORT_PARAM_PACKAGE "PACKAGE" 
#define REPORT_PARAM_APP "APP" 
#define REPORT_PARAM_ALL "ALL"


#define RET_STATUS "status" 
#define RET_OK "OK" 
#define RET_ERR "ERR" 
#define RET_MESSAGE "message" 
// intv:
#define APP_HB_INT 30 //S

#define PORT_DEFAULT 12345
#define PORT_LEN 500
//ADMIN USER
#define ADMIN_USER "geoeast"
#endif
