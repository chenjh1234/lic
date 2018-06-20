/*
 * tetrisd.cpp
 *
 *  Created on: 2016年6月23日
 *      Author: lxd
 */

#include <distributed_bus.hpp>
#include <tetris_um.hpp>

#include <tetris_service_register.hpp>
#include <tetris_jm_master.hpp>
#include <tetris_resource_manager.hpp>
#include <tetris_sm.hpp>
/* the simulated client device */
class client_device : public tetris::device
{
public:
	client_device (bool _debug_mode=true) :
			debug_mode(_debug_mode),
			should_exit(false),
			the_caller(NULL),
			count(0)
	{}
private:
	bool startup (helpers* hp)
	{
		if (!tetris::device::startup (hp))
			return false;
		the_caller = new boost::thread (boost::bind (&client_device::caller, this));
		return the_caller != NULL;
	}
	void shutdown (void)
	{
		should_exit = true;
		if (the_caller) {
			the_caller->join ();
			delete the_caller;
		}
		cout<<"client shutdown "<<endl;
	}
private:
	void caller (void)
	{
		/* we can omit the last parameter. it defaults to sn_any */
		tetris::device::id the_server (10,0,0/*,tetris::device::id::sn_any*/);
		tetris::device::id rm_server ("2:0:0:0");
//		tetris::device::id the_server1 ("1:0:0:0");
		Json::Value params, result;
		struct timeval t1, t2;
		gettimeofday (&t1, NULL);
		while (!should_exit) {
			/*test get_devices*/
			tetris::device::id fliter(-2,-2,-2,-2);//self
//			tetris::device::id fliter(-1,-1,-1,-1);
			Json::Value devices;

			hp->async_call (the_server, 0, "hello", params,
							string (), tetris::never_timeout,
							MAKE_DEVICE_ASYNC_CALLBACK (client_device, async_callback));
			if (true)
				sleep (1);
		}
		gettimeofday (&t2, NULL);
		double delta = t2.tv_sec-t1.tv_sec+(t2.tv_usec-t1.tv_usec)/1000000.f;
		TETRIS_DEBUG("%d calls made, cps: %0.1f", count, count / delta);
	}
	DECLARE_DEVICE_ASYNC_CALLBACK (async_callback)
	{
		if (finished == true) {

			/* async callbacks also need to deal with locking */
			//boost::mutex::scoped_lock lock (count_mutex);
			//cout<<"client callback response="<<result["response"]<<endl;
			count++;
		}
	}
private:
	bool debug_mode;
	bool should_exit;
	boost::thread* the_caller;
	int count;
	boost::mutex count_mutex;

};



/* the simulated server device */
class server_device : public tetris::device{
public:
	server_device () : count(0) {}
private:
	unsigned get_methods (const unsigned& function, method_map& methods)
	{
		/* we have only one method, too */
		if (function == 0){
			ADD_DEVICE_METHOD (methods, server_device, hello);
	    	ADD_DEVICE_METHOD (methods, server_device, Get);}
		return methods.size ();
	}
	bool startup (helpers* hp)
	{
		/* startup the base and remember the time */
		if (tetris::device::startup (hp)) {
			gettimeofday (&startup_time, NULL);
			return true;
		}
		return false;
	}
	void shutdown (void)
	{
		/* tell people how fast we can go */
		gettimeofday (&shutdown_time, NULL);
		double delta = shutdown_time.tv_sec-startup_time.tv_sec +
				(shutdown_time.tv_usec-startup_time.tv_usec)/1000000.f;
		TETRIS_DEBUG("%d requests serviced, rps: %0.1f", count, count / delta);
	}
private:
	DECLARE_DEVICE_METHOD (hello)
	{
		/* device methods are called by the bus cocurrently, so
		 * they have to deal with locking while accessing shared variables */
		boost::mutex::scoped_lock lock (count_mutex);
		result["response"]=count;
		Json::Value p,q;
		tetris::device::id the_server (10,0,0/*,tetris::device::id::sn_any*/);

		count++;
		return tetris::device::method_finished;
	}
	DECLARE_DEVICE_METHOD (Get)
	{
		/* device methods are called by the bus cocurrently, so
		 * they have to deal with locking while accessing shared variables */
		boost::mutex::scoped_lock lock (count_mutex);
		Json::Value jobjson;
		jobjson["name"]="job1";
		result.append(jobjson);
		return tetris::device::method_finished;
	}
private:
	int count;
	boost::mutex count_mutex;
	struct timeval startup_time, shutdown_time;

};


int should_exit = 0;
void sighandler (int /*signum*/)	{
	TETRIS_DEBUG("sighandler");
	should_exit = 1;
}


int main (int argc, char *argv []){

	string tetris_root;
	string un_useD;
	int un_useP;
	X509* ca_cert;
	tetris::device * device_ptr;	/*device pointer*/
	string cacert_file,device_conf_file,user_conf_file,device_cert_file,device_key_file;
	Json::Value bus_options;

	string pidfile = "/var/run/tetrisd";
	if (!global::singleton (pidfile.c_str())) {
		cout<<"Error: tetrisd already running!\n";
		return -1;
	}

	/* process the command line options ,and store in bus_options*/
	if(global::parseOptions(argc,argv,bus_options,tetris_root,un_useD,un_useP)!=0){
		cout<<"ERROR: Cannot parse options!\n";
		return -1;
	}
	cacert_file = tetris_root+CACERT_FILE;
	device_conf_file = tetris_root+DEVICE_CONF_FILE;
	user_conf_file = tetris_root+USER_CONF_FILE;
	device_cert_file = tetris_root+DEVICE_CERT_FILE;
	device_key_file = tetris_root+DEVICE_KEY_FILE;
	/* init the openssl library */
	OpenSSL_add_all_algorithms ();
	ERR_load_crypto_strings ();
	init_ssl_locks ();

	/* load the root certificate/private key */
	if (!(ca_cert = tetris::pki::load_cert (cacert_file))) {
		cout<<"ERROR: Cannot load the root cert!\n";
		return -1;
	}
	signal (SIGINT, sighandler);

	Json::Value configure;
	int seconds;
	if(!global::loadJsonConf(configure)){
		cout<<"Error: configure file load failed!\n";
		return -1;
	}
	if(configure.isMember("DEVICE_ATTACH_INTERVAL") && configure["DEVICE_ATTACH_INTERVAL"].isInt()){
		seconds=configure["DEVICE_ATTACH_INTERVAL"].asInt();
	}else{
		cout<<"Error: DEVICE_ATTACH_INTERVAL is not set,please set in configuration file!\n";
		return -1;
	}


	/*step1:init bus*/
	/* create a new bus and initialize it */
	tetris::bus* bus = tetris::new_bus ("distributed");
	if (bus == NULL){
		cout<<"ERROR: Cannot create the bus!\n";
		return -1;
	}
	if (!bus->init (ca_cert, bus_options, NULL)) {
		TETRIS_ERROR("Cannot init the bus.");
		return -1;
	}
	if(tetris_debug_enable==1){
		TETRIS_DEBUG("\n\n\n\n debug mode is open!!\n\n\n\n");}
	else{
		TETRIS_DEBUG("\n\n\n\n debug mode is close!!\n\n\n\n");}

	/* start up the bus.
	 * devices can be attached/detached anytime after the bus started */

	if (!bus->startup ()) {
		TETRIS_ERROR("Cannot start the bus.");
		return -1;
	}

	/*attach register device which for generateing registering call and save certificate*/
	tetris_service_register rd(ca_cert,device_conf_file,device_cert_file,device_key_file,"");
	tetris::bus::cd_login rd_cd = {"whatever", "anything"};
	bus->attach_device (&rd, rd_cd);
	TETRIS_DEBUG("Attach tetris_service_register.");

	device_str_map attached_func_devices,attached_user_devices;
	/* wait for the 'Ctrl+C' */
	while (!should_exit){

		device_str_set func_devices,user_devices;
		device_str_set ready_func_devices,detach_func_devices;
		device_cd_map devices_cd;

		/*parse all device configure,which is intended to attach
		 * funtional device file name like: 1:0:0:0*/
		global::scanFileName(device_conf_file,func_devices);

		/*load all device's cert and private key
		 * cert and key file name like:2:0:0:0*/
		global::loadDeviceCd(device_cert_file,device_key_file,devices_cd);

		/*find out ready devices and attach*/
		for(device_str_set::iterator it=func_devices.begin();it!=func_devices.end();++it)
		{
			string id_str = *it;
			if(attached_func_devices.find(id_str)!=attached_func_devices.end()){
				TETRIS_DEBUG("Already attach device# %s \n",id_str.c_str());
				continue;
			}
			vector <string> vs;
			boost::split (vs, id_str, boost::is_any_of (":"));
			if (vs.size () != 4)
				continue;
			if(devices_cd.find(id_str)!=devices_cd.end()){
				tetris::device::id id(id_str);
				tetris::bus::cd_certificate cd = devices_cd[id_str];
				switch(id.u.s.service){
				case global::service_resource_manager:
						device_ptr = new tetris_resource_manager;
						if(bus->attach_device(device_ptr,cd)){
							attached_func_devices[id_str]=device_ptr;
							TETRIS_DEBUG("tetris_resource_manager attach succeed.");
						}
						else
							TETRIS_ERROR("tetris_resource_manager attach failed.");
						break;
				case global::service_job_master://4
						device_ptr = new tetris_job_manager_master;
						if(bus->attach_device(device_ptr,cd)){
							attached_func_devices[id_str]=device_ptr;
							TETRIS_DEBUG("tetris_job_manager_master attach succeed.");
						}else
							TETRIS_ERROR("tetris_job_manager_master attach failed.");
						break;
				case global::service_service_manager://3
						device_ptr = new tetris_service_manager;
						if(bus->attach_device(device_ptr,cd)){
								attached_func_devices[id_str]=device_ptr;
								TETRIS_DEBUG("tetris_service_manager attach succeed.");
						}else
							TETRIS_ERROR("tetris_service_manager attach failed.");
						break;
				case 10:
					device_ptr = new server_device;
					if(bus->attach_device(device_ptr,cd)){
						 attached_func_devices[id_str]=device_ptr;
						 TETRIS_DEBUG("server_device attach succeed.");
					}else
						 TETRIS_ERROR("server_device attach failed.");
				  break;
				default:
					break;
				}//switch
			}//if
		}//for

		/*find out detach functional devices and detache*/
		for(device_str_map::iterator it=attached_func_devices.begin();
				it!=attached_func_devices.end();++it)
		{
				string id_str = it->first;
				if(func_devices.find(id_str)==func_devices.end()){
					bus->detach_device(attached_func_devices[id_str]);
					delete attached_func_devices[id_str];
					attached_func_devices.erase(id_str);
					TETRIS_DEBUG("detach device# %s ",id_str.c_str());
				}
		}

		/*stepN:attach non-functional device
		 *user device file name like: class_name:user_name*/
		global::scanFileName(user_conf_file,user_devices);
		for(device_str_set::iterator it = user_devices.begin();it!=user_devices.end();++it)
		{
			string id_str = *it;
			if(attached_user_devices.find(id_str)!=attached_user_devices.end()){
			TETRIS_DEBUG("Already attach device# %s",id_str.c_str());
				continue;
			}
			string device_name,user_name;
			vector <string> vs;
			boost::split (vs, id_str, boost::is_any_of (":"));
			if (vs.size () != 2)
				continue;
			device_name = vs[0];
			user_name= vs[1];
			tetris::bus::cd_login cd = {vs[1],"anything"};
			int user_device_id;
			stringstream ss(device_name);
			ss>>user_device_id;
			if(ss.fail()){
				TETRIS_ERROR("transfer device id: %s from string to int.",id_str.c_str());
				continue;
			}
			switch(user_device_id){
			case global::client_test:
				device_ptr = new client_device ;
				if(bus->attach_device(device_ptr,cd)){
					attached_user_devices[id_str]=device_ptr;
					TETRIS_DEBUG("client_device attach succeed.");
				}else
					TETRIS_ERROR("client_device attach failed.");
				break;
			default:	break;
			}//switch
		}//for

		/*find out detach user devices and detache*/
		for(device_str_map::iterator it=attached_user_devices.begin();
				it!=attached_user_devices.end();++it){
				string id_str = it->first;
				if(user_devices.find(id_str)==user_devices.end()){
					bus->detach_device(attached_user_devices[id_str]);
					delete attached_user_devices[id_str];
					attached_user_devices.erase(id_str);
					TETRIS_DEBUG("detach user device# %s. ",id_str.c_str());
				}
		}
		sleep (seconds);
	}//while

	TETRIS_DEBUG("tetrisd terminating......");
	/* this will detach/shutdown all devices automatically */
	bus->detach_device(&rd);
	tetris::delete_bus (bus);

	TETRIS_DEBUG("tetrisd terminating.....tetris::delete_bus (bus);.");
	return 0;
}
