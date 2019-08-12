
#ifndef LICAGENT_HPP__ 
#define LICAGENT_HPP__ 

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
#include "LFileDate.h"
#include "SAgent.h"
#include "SCycleThread.h"
#include "jsonParser.hpp"
#include "comDefs.h"
//#include "licServer.hpp"
#include <QString>
#include <QStringList>
//#include <tetris/init_ssl.hpp>
//debug setting

#define the_server *_devServer


#define SERVER_LIB_EXPORT Q_DECL_EXPORT
/* the simulated server device */
class SERVER_LIB_EXPORT LicAgent: public tetris::device
{
public:
	LicAgent ();//: count(0) {}
        int _down;
private:
        unsigned get_functions (void) ;
	unsigned get_methods (const unsigned& function, method_map& methods);
        processor_type get_processor (const unsigned& function)
        {
   /* we have only one method, too */
  // if (function == 0)
                 return MAKE_DEVICE_PROCESSOR(LicAgent,pro);   
        };
         
	bool startup (helpers* hp);
	void shutdown (void);
        //SCycleThread cyc;
        SAgent mng;
        
	
private:
    // method:test

	DECLARE_DEVICE_METHOD (hello);//1
    // real interface:
        DECLARE_DEVICE_METHOD (loginApp);//2
        DECLARE_DEVICE_METHOD (logoutApp);//3
        DECLARE_DEVICE_METHOD (heartBeat);//4
        DECLARE_DEVICE_METHOD (shutDown);//5
        DECLARE_DEVICE_METHOD (loadConfig);//6
        DECLARE_DEVICE_METHOD (viewConfig);//7
        DECLARE_DEVICE_METHOD (clearConfig);//8
        DECLARE_DEVICE_METHOD (viewAgent);//9
        //
        //DECLARE_DEVICE_METHOD (checkUUID);
    // real interfase end:
        //processor
        DECLARE_DEVICE_PROCESSOR(pro);


        QStringList loginApp(SAppInfo &app);
        int unloginApp(SAppInfo &app);
        QStringList serverLogin(SAppInfo &app);
        int serverLogout(SAppInfo &app);
        int checkHB(long intvs);
        int checkHB(QString pack,long intvs);

        int thCyc();
        bool isDown();
     



        Json::Value   toJason(SAppInfo &info);
        QString  getJsonV(Json::Value& jv, QString key);

        void setDev(QString dev);
        tetris::device::id  & getDev(){return *_devServer;};

        tetris::device::id * _devServer;
        boost::thread  * the_cyc;

private:
	long count;
	boost::mutex count_mutex;
	struct timeval startup_time, shutdown_time;
        bool _startupFailed;
};
#endif

