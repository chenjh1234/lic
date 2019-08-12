
#ifndef LICSERVER_HPP__ 
#define LICSERVER_HPP__

#include <stdio.h>
#include <sys/time.h>
#include <tetris/device.hpp>
//#include "distributed_bus.hpp"
//#include "util.hpp"
#include <set>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <uuid/uuid.h>
#include <tetris/bus.hpp>
#include <dirent.h>
#include "SLicMng.h"
#include "SCycleThread.h"
#include "jsonParser.hpp"
#include "comDefs.h"
#include <QString>
#include <QStringList>
//#include <tetris/init_ssl.hpp>
//debug setting

#define SERVER_LIB_EXPORT Q_DECL_EXPORT
/* the simulated server device */
class SERVER_LIB_EXPORT LicServer : public tetris::device
{
public:
	LicServer () : count(0) {}
        int _down;
private:
        unsigned get_functions (void) ;
	unsigned get_methods (const unsigned& function, method_map& methods);
        processor_type get_processor (const unsigned& function)
        {
   /* we have only one method, too */
  // if (function == 0)
                 return MAKE_DEVICE_PROCESSOR(LicServer,pro);   
        };
         
	bool startup (helpers* hp);
	void shutdown (void);
        SCycleThread cyc;
        
	
private:
    // method:test

	DECLARE_DEVICE_METHOD (hello);//1
    // real interface:
        DECLARE_DEVICE_METHOD (loadFile);//2
        DECLARE_DEVICE_METHOD (unloadFile);//3
        DECLARE_DEVICE_METHOD (unloadPackage);//4
        DECLARE_DEVICE_METHOD (removePackage);//5
        DECLARE_DEVICE_METHOD (loginApp);//6
        DECLARE_DEVICE_METHOD (logoutApp);//7
        DECLARE_DEVICE_METHOD (heartBeat);//8
        DECLARE_DEVICE_METHOD (borrow);//9
        DECLARE_DEVICE_METHOD (borrowReturn);//10
        DECLARE_DEVICE_METHOD (report);//11
        DECLARE_DEVICE_METHOD (shutDown);//12
        DECLARE_DEVICE_METHOD (loadConfig);//13
        DECLARE_DEVICE_METHOD (viewConfig);//14
        DECLARE_DEVICE_METHOD (clearConfig);//15
        //
        //DECLARE_DEVICE_METHOD (checkUUID);
    // real interfase end:
        //processor
        DECLARE_DEVICE_PROCESSOR(pro);

private:
	long count;
	boost::mutex count_mutex;
	struct timeval startup_time, shutdown_time;
        bool _startupFailed;
};
#endif
