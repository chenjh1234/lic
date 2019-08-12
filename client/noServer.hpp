#ifndef NOSERVER_HPP__ 
#define NOSERVER_HPP__

#include <stdio.h>
#include <sys/time.h>
#include <tetris/device.hpp>
#include <boost/thread/thread.hpp>

#include <dirent.h>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QThread>
#include <http_call.hpp>
#include "LFileDate.h"
#include "SAppInfo.h"
#include "comDefs.h"
#include "jsonParser.hpp"
//#include <tetris/init_ssl.hpp>
//debug settin
//#define MY_LIC_SERVER tetris::device::id the_server(2, 0, 0/*,tetris::device::id::sn_any*/);

#define the_server *_devServer



#define HTTP_CLIENT_START(x) \
    if(isPortal()) \
    {\
        string ip= _portalIp.Q2CH;\
	unsigned short port;\
        port = _portalPort.toUShort();\
	HttpClient hc(ip,port);\
        hc.init();\
        qDebug() << "In Portl===port " << port;\
        bool b = hc.Call(LIC_URL, x,params,result);\
        hc.stop();\
    }else

 



 
class HbThread;

/* the simulated client device */
class NoServer : public tetris::device
{
public:
	NoServer (bool _debug_mode=true) :
			debug_mode(_debug_mode),
			should_exit(false),
			the_caller(NULL),
			count(0)
	{init();};
      ~NoServer();

      void init();
 //loadfiles:
      int loadFile(QString filename);
      int loadConfig(QString filename);
      int clearConfig(QString passwdtoday);
      int unloadFile(QString filename,QString proofFile);
// load unload pack:
      QString unloadPackage(QString vender,QString pack,QString version,int number=0,QString ty="",QString uuid = "");
      QString removePackage(QString vender,QString pack,QString version,int number,QString ty ,QString uuid ,QString pass);
//login logout:
      QStringList loginApp(QString vender,QString packName,QString version,int number=1,QString appname="");
      int logoutApp(QString vender,QString packName,QString version);
      int logoutApp();
// HB
      void startHB();
      void endHB();
      void heartBeat(QString vender,QString packName,QString version);
      void heartBeat();
      void startHB(QString vender,QString pack,QString version);
      void stopHB(QString vender,QString pack,QString version);
      QString encodeHB(QString vender,QString pack,QString version);
      QStringList  decodeHB(QString en);
// borrow:
      int borrow(QString filein,QString fileout);
      int borrowReturn(QString proofFile);

      QStringList report(QString param=REPORT_PARAM_PACKAGE   ,QString param1="" );
      QStringList viewConfig(QString pack="");
      QStringList viewAgent(QString pack="");
// shutdown:
      void shutDown(); // shutdown interface
      //bool  setCheckUUID(bool b);
// dev:
      QString getDev(QString id);

      void setDev(QString dev);
      tetris::device::id  & getDev(){return *_devServer;};
      void setPortal(int i=0,QString ip="",QString port="");

      bool isPortal();// yes connect portal
      bool isAgent();
    

private:
	bool startup (helpers* hp);
	void shutdown (void);// for device down
        HbThread *hbTH;
        QString _ip ,_user,_hostname,_pid,_licproject;
        QString _portalIp,_portalPort;
        QString _licAgent;
        int _portal;
  
private:
	void caller (void);
        //void hber (void);
	DECLARE_DEVICE_ASYNC_CALLBACK (async_callback);	
private:
	bool debug_mode;
	bool should_exit;
	boost::thread* the_caller;
       // boost::thread* the_hber;
	long count;
	boost::mutex count_mutex;
        tetris::device::id * _devServer;


        QMap<QString,QString> mapHB;
        QMap<QString,QStringList> mapLogin;// for login register, if the same proess login packid more times return the first;
         
};
//=====================================================================
class HbThread : public QThread
{
public:

    HbThread():QThread(){_down = 0;};
    void run()
    {
        int ic ;
       
        while (1) 
        {
            ic = 0;
            while (ic < APP_HB_INT) 
            {
                ic++;
                sleep(1);
                if (_down == 1) break;
            }
            if (_down == 1) break;
            if (client != NULL)  client->heartBeat();
            if (_down == 1) break;
        }
    }; 
    void setDown(){_down = 1;};
    void setClient(NoServer *c)
    {
        client = c;
    };
private:
    NoServer *client;
    int _down;

};
#endif

#if 0 // test boost::asio::deadline_timer

void test_timer_syn()  
{  
    boost::asio::io_service ios;  
    boost::asio::deadline_timer t(ios, boost::posix_time::seconds(3));  
    cout << "wait " << t.expires_at() << endl;  
    t.wait();  
    cout << "wait ok\n" ;
}  
void handle_wait(const boost::system::error_code& error,
                 boost::asio::deadline_timer& t,
                 int& count)
{
   if (!error)
   {
      cout << "count = " << count << endl;
      if (count++ < 5)
      {
         t.expires_from_now(boost::posix_time::seconds(3));
         t.async_wait(boost::bind(handle_wait,
                                  boost::asio::placeholders::error,
                                  boost::ref(t),
                                  boost::ref(count)));
         if (count == 3)  
         {
            t.cancel();
         }

      }
   }
}
void print(const boost::system::error_code& e)
{
    cout << "print\n";
}
void test_timer_asyn()  
{  
    boost::asio::io_service io;  
  
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(3));  
    cout << "before async_wait...\n";  
    t.async_wait(print);  
    cout << "After async_wait...\n";  
    io.run(); //wait here 
}  
void test_timer_asyn_loop()
{
   boost::asio::io_service io;
   boost::asio::deadline_timer t(io);
   t.expires_from_now(boost::posix_time::seconds(1));

   int count = 0;
   t.async_wait(boost::bind(handle_wait,
                            boost::asio::placeholders::error,
                            boost::ref(t),
                            boost::ref(count)));
   cout << "loop wait\n";
   io.run();// here wait 1s to run handle:
   cout << "loop out\n";
}
main()
{
    test_timer_asyn_loop();
    test_timer_syn() ;
    test_timer_asyn() ;
    cout << "ok end\n";
}
#endif
