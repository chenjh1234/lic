/*
* portal.cpp
 *
 *  Created on: 2016年11月28日
 *      Author: lxd
 *      detail:port()
 *       		main--> class
 *       		cert file path
 invalid use of ?class std::set<typename function_t::arg<Indices>...>
 */


#ifndef TETRIS_TETRIS_PORTAL_HPP_
#define TETRIS_TETRIS_PORTAL_HPP_

#include "include/crow_all.h"

#include <distributed_bus.hpp>
#include <tetris/device.hpp>
#include <signal.h>
#include <tetris/init_ssl.hpp>
#include <json/json.h>

#include <tetris/global_setting.hpp>
 
#include "comDefs.h"
 

#define CMD_TIMEOUT boost::posix_time::time_duration (0,0,30)
int should_exit = 0;
void sighandler (int /*signum*/)	{ should_exit = 1; }

tetris::device *d;

set<string> JMProvideMethods;
set<string> SMProvideMethods;
set<string> RMProvideMethods;
set<string> UMProvideMethods;
set<string> LMProvideMethods;

string jm_id,sm_id,rm_id,lm_id;//add

class tetris_service_gateway : public tetris::device {
public:
    tetris_service_gateway(int & port):
    	the_caller(NULL),
    	portalPort(port)
    	{ }
    ~tetris_service_gateway() { }

    /* bus interface */
private:
    bool startup(helpers *hp) {
    	Json::Value configure;
    	if(!global::loadJsonConf(configure)){
    		TETRIS_ERROR("Error: configure file load failed.");
    		return false;
    	}
    	if(configure.isMember("device_id_jm") && configure["device_id_jm"].isString()){
    		jm_id=configure["device_id_jm"].asString();
    	}
    	if(configure.isMember("device_id_rm") && configure["device_id_rm"].isString()){
    		rm_id=configure["device_id_rm"].asString();
    	}
    	if(configure.isMember("device_id_sm") && configure["device_id_sm"].isString()){
    		sm_id=configure["device_id_sm"].asString();
    	}
        if(configure.isMember("device_id_lm") && configure["device_id_lm"].isString()){
    		lm_id=configure["device_id_lm"].asString();
    	}

    	if (!tetris::device::startup(hp))
    		return false;
    	string jm[]={"Get","Delete","Archive","Add","Modify","Trigger","Cancel"};
    	JMProvideMethods=set<string>(jm,jm+7);
    	string sm[]={"getService","deleteService","deployService","deployTetrisd","remoteCall"};
    	SMProvideMethods=set<string>(sm,sm+4);
    	string rm[]={"call"};
    	RMProvideMethods=set<string>(rm,rm+1);
    	string um[]={"approvalCert"};
    	UMProvideMethods=set<string>(um,um+1);
        string lm[]={"call"};
    	LMProvideMethods=set<string>(lm,lm+1);

        /*start work threads*/
    	the_caller= new boost::thread (boost::bind (&tetris_service_gateway::processRestfulCall, this));
        /*thread success return 1 */
      return the_caller != NULL;
      }
    void shutdown(void){
    	TETRIS_ERROR("tetris_service_gateway shutdown.");
    	should_exit = true;
    	if (the_caller) {
    		the_caller->join ();
    		delete the_caller;}
    }

public:
    /*crow thread*/
	boost::thread* the_caller;
public:
   void processRestfulCall(void);
private:
   int portalPort;


};


void tetris_service_gateway::processRestfulCall(void) {

	crow::SimpleApp app;
   d=this;
   CROW_ROUTE(app, "/")
     ([]() {return crow::response{"web server working...\n"};});

/*restful api for job manager
 * {
 * 		"method":x
 * 		"params":{"type":x}
 * }
 * */
#if 0
      CROW_ROUTE(app, "/job/call")
     ([]() {
          cout << "this is job call\n";

          return crow::response{"web server working...\n"};});
#endif	 
 
	CROW_ROUTE(app, "/job/call")
    	.methods("POST"_method)
        ([](const crow::request& req){
   
            cout <<"this is job call\n";
       	Json::Value body_json,params, result;
       	auto x = crow::json::load(req.body);
       	std::ostringstream x_os;
       	x_os << x;
       	string body_string=x_os.str();
       	string method;

       	Json::Reader reader;
       	if(!reader.parse(body_string,body_json)){
       		return crow::response{"ERROR: parameters is invalid."};
       		}
       	if((!body_json.isMember("params"))||(!body_json.isMember("method")) ||
       			(!body_json["method"].isString()) || (!body_json["params"].isObject())){
       		return crow::response{"ERROR: parameters is invalid."};
       		}
       	method=body_json["method"].asString();
       	params=body_json["params"];
       	TETRIS_DEBUG("jm/call body = %s", body_json.toStyledString().c_str());
       	if(JMProvideMethods.find(method)==JMProvideMethods.end()){
       		return crow::response{"ERROR: Not supported Method"};
       		}

       	tetris::device::id the_server(jm_id);
       	d->hp->call (the_server, 0, method, params,string (),tetris::second_5,result);
       	TETRIS_DEBUG("jm/call result = %s", result.toStyledString().c_str());
       	return crow::response{result.toStyledString()};
    });
 
        // add cjh:
        CROW_ROUTE(app, "/lm/call")
    	  .methods("POST"_method)
   ([](const crow::request& req){
            cout << "lm call entered --------------\n";

       	Json::Value body_json,params, result;
       	auto x = crow::json::load(req.body);
       	std::ostringstream x_os;
       	x_os << x;
       	string body_string=x_os.str();
       	string method;
        string agent;
        

       	Json::Reader reader;
       	if(!reader.parse(body_string,body_json)){
       		return crow::response{"ERROR: parameters is invalid."};
       		}
       	if((!body_json.isMember("params"))||(!body_json.isMember("method")) ||
       			(!body_json["method"].isString()) || (!body_json["params"].isObject())){
       		return crow::response{"ERROR: parameters is invalid."};
       		}
       	method=body_json["method"].asString();
       	params=body_json["params"]["url_params"];
       	TETRIS_DEBUG("lm/call body = %s", body_json.toStyledString().c_str());
       	if(LMProvideMethods.find(method)==LMProvideMethods.end()){
       		return crow::response{"ERROR: Not supported Method"};
       		}

   
        
//agent:// get divice id from param"agent"
        string lm_idd;
        lm_idd = lm_id;
        agent = params["licagent"].asString();
        if (!agent.empty()) lm_idd= agent;
//device: licServer or licAgent
        tetris::device::id the_server(lm_idd);

        cout << "server id = " << lm_id << lm_idd << agent << method<<endl ;
// method:

        method = body_json["params"]["url"].asString();
// notify or call:
        if (params["notify"].asString() =="yes")  
        {
            d->hp->notify(the_server, 0, method, params, string(), tetris::second_10); 
            result["status"] = "OK";
            cout << "result of call = " << result.toStyledString().c_str() << endl;
            return crow::response{result.toStyledString()};
        }
        else
        {
            d->hp->call(the_server, 0, method, params, string(), tetris::second_10,result); 
            TETRIS_DEBUG("lm/call result = %s", result.toStyledString().c_str());
             cout << "result of call = " << result.toStyledString().c_str() << endl;
            return crow::response{result.toStyledString()};
        }
 
    });

	CROW_ROUTE(app, "/sm/call")
    	  .methods("POST"_method)
   ([](const crow::request& req){

       	Json::Value body_json,params, result;
       	auto x = crow::json::load(req.body);
       	std::ostringstream x_os;
       	x_os << x;
       	string body_string=x_os.str();
       	string method;
       	Json::Reader reader;
       	if(!reader.parse(body_string,body_json)){
       		return crow::response{"ERROR: parameters is invalid."};
       		}
       	if((!body_json.isMember("params"))||(!body_json.isMember("method")) ||
       			(!body_json["method"].isString()) || (!body_json["params"].isObject())){
       		return crow::response{"ERROR: parameters is invalid."};
       		}
       	method=body_json["method"].asString();
       	params=body_json["params"];
       	TETRIS_DEBUG("sm/call body = %s", body_json.toStyledString().c_str());
       	if(SMProvideMethods.find(method)==SMProvideMethods.end()){
       		return crow::response{"ERROR: Not supported method."};
       		}
       	tetris::device::id the_server (sm_id);
       	d->hp->call (the_server, 0, method, params,string (),tetris::second_10,result);
       	TETRIS_DEBUG("sm/call result = %s", result.toStyledString().c_str());
       	return crow::response{result.toStyledString()};
    });
	CROW_ROUTE(app, "/rm/call")
    	  .methods("POST"_method)
   ([](const crow::request& req){

       	Json::Value body_json,params, result;
       	auto x = crow::json::load(req.body);
       	std::ostringstream x_os;
       	x_os << x;
       	string body_string=x_os.str();
       	string method;

       	Json::Reader reader;
       	if(!reader.parse(body_string,body_json)){
       		return crow::response{"ERROR: parameters is invalid."};
       		}
       	if((!body_json.isMember("params"))||(!body_json.isMember("method")) ||
       			(!body_json["method"].isString()) || (!body_json["params"].isObject())){
       		return crow::response{"ERROR: parameters is invalid."};
       		}
       	method=body_json["method"].asString();
       	params=body_json["params"];

       	TETRIS_DEBUG("rm/call body = %s", body_json.toStyledString().c_str());
       	if(RMProvideMethods.find(method)==RMProvideMethods.end()){
       		return crow::response{"ERROR: Not supported method."};
       		}
//		Json::Value rm_params,url_params,job_params,rm_result;
//		job_params["type"]=1;
//		url_params["method"]="Get";
//		url_params["params"]=job_params;
//		rm_params["url"]="192.168.2.128:18080/job/call";
//		rm_params["url_params"]=url_params;
//		tetris::device::id the_server (2,0,0/*,tetris::device::id::sn_any*/);
//	       	d->hp->call (the_server, 0, "call", rm_params,string (),tetris::never_timeout,result);

       	tetris::device::id the_server (rm_id);
       	d->hp->call (the_server, 0, method, params,string (),tetris::second_10,result);
//       	for(int i=0;i<result["count"].asInt();i++)
//       		cout<<result["jobList"][i]["jobName"]<<endl;
       	TETRIS_DEBUG("rm/call result = %s", result.toStyledString().c_str());

//    		Json::FastWriter writer;
//    		string tmp=writer.write(result);
//    		return crow::response{tmp};
       	return crow::response{result.toStyledString()};
    });


	CROW_ROUTE(app, "/um/call")
    	  .methods("POST"_method)
   ([](const crow::request& req){

       	Json::Value body_json,params, result;
       	auto x = crow::json::load(req.body);
       	std::ostringstream x_os;
       	x_os << x;
       	string body_string=x_os.str();
       	string method;
       	Json::Reader reader;
       	if(!reader.parse(body_string,body_json)){
       		return crow::response{"ERROR: parameters is invalid."};
       		}
       	if((!body_json.isMember("params"))||(!body_json.isMember("method")) ||
       			(!body_json["method"].isString()) || (!body_json["params"].isObject())){
       		return crow::response{"ERROR: parameters is invalid."};
       		}
       	method=body_json["method"].asString();
       	params=body_json["params"];
       	TETRIS_DEBUG("um/call body = %s", body_json.toStyledString().c_str());
       	if(UMProvideMethods.find(method)==UMProvideMethods.end()){
       		return crow::response{"ERROR: Not supported method."};
       		}
       	tetris::device::id the_server ("1:0:0:-1");
       	d->hp->call (the_server, 0, method, params,string (),tetris::second_10,result);
       	TETRIS_DEBUG("um/call result = %s", result.toStyledString().c_str());
       	return crow::response{result.toStyledString()};
    });

	//app.port(40080).multithreaded().run();
	app.port(portalPort).multithreaded().run();
	TETRIS_DEBUG("crow stop.");
	should_exit = 1;
}

 

int main (int argc, char *argv []){

 
	char * tetris_root=getenv("TETRIS_ROOT");
	string tetrisRoot,un_use;
	int portalPort=18080;
	if(tetris_root==NULL){
			TETRIS_ERROR("Please set TETRIS_ROOT environment first.");
			return -1;
		}

	string tr(tetris_root);
	string cacert_file;


	X509* ca_cert;
	Json::Value bus_options;

	/* process the command line options ,and store in bus_options*/
	if(global::parseOptions(argc,argv,bus_options,tetrisRoot,un_use,portalPort)!=0){
		TETRIS_ERROR("ERROR: Cannot parse options.");
		return -1;
	}

	cacert_file = tetrisRoot+CACERT_FILE;
	/* init the openssl library */
	OpenSSL_add_all_algorithms ();
	ERR_load_crypto_strings ();
	init_ssl_locks ();
string a1 = CA_CERT;
	/* load the root certificate/private key */
	if (!(ca_cert = tetris::pki::load_cert (CA_CERT))) {
		TETRIS_ERROR("ERROR: Cannot load the root cert.");
		return -1;
	}
	signal (SIGINT, sighandler);
        cout << "port= " << portalPort;

      //  portalPort=18080;
        int busport = 12310;

       bus_options["unicast_port"] = (unsigned int)busport;

	/*step1:init bus*/
	/* create a new bus and initialize it */
	tetris::bus* bus = tetris::new_bus ("distributed");
	if (bus == NULL){
		TETRIS_ERROR("ERROR: Cannot create the bus.");
		return -1;
	}
	if (!bus->init (ca_cert, bus_options, NULL)) {
		TETRIS_ERROR("ERROR: Cannot init the bus.");
		return -1;
	}
	/* start up the bus*/

	if (!bus->startup ()) {
		TETRIS_ERROR("ERROR: Cannot start the bus.");
		return -1;
	}
	/* start up the gateway*/
   tetris_service_gateway   gw(portalPort);
   tetris::bus::cd_login cd1 = {"whatever", "anything"};
   bus->attach_device(&gw,cd1);

   while(!should_exit)
	   sleep(1);

	TETRIS_ERROR("portal stop.");
   /* this will detach/shutdown all devices automatically */
	tetris::delete_bus (bus);
	return 0;
   
}

#endif /* TETRIS_PORTAL_HPP_ */
