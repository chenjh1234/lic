#ifndef START_CLIENT_H__ 
#define START_CLIENT_H__ 

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <dirent.h>
#include <signal.h>
#include <json/json.h>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <tetris/bus.hpp>
#include <tetris/device.hpp>
//#include <tetris/init_ssl.hpp>
 
#include "noServer.hpp"
 
#if 0
#define TETRISD_DEBUG_ENABLED			true
#define debug_x(fmt,args...) \
	if(TETRISD_DEBUG_ENABLED)\
		printf(fmt,args);
#endif
#define CLIENT_LIB_EXPORT Q_DECL_EXPORT
/// find a free port to used,and start the bus->noserver
class CLIENT_LIB_EXPORT StartClient
{
public:
    StartClient();
    ~StartClient();
    int initDev();
    int initDev1();
    int closeDev();
    tetris::bus* bus;
   // user_manager_device *um ;
    NoServer *client;

    void setPortal(int idx);
    bool isPortal();
    void  getPortal();// get env of portalip,port; ip exist setPortal(1);else 0
    //int getFreePort();
    QString _portalIp,_portalPort;
    int _portal;

};

#endif
