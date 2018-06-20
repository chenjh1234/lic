/*
 * tetrisc.cpp
 *
 *  Created on: 2016年6月29日
 *      Author: lxd
 */


#include <distributed_bus.hpp>
#include <tetris_um.hpp>

#include <tetris_service_register.hpp>
//#include <tetris_jm_master.hpp>
#include <tetris_resource_manager.hpp>
//#include <tetris_service_gateway.hpp>



using namespace std;

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
		TETRIS_DEBUG("client shutdown ");
	}
private:
	void caller (void)
	{
		/* we can omit the last parameter. it defaults to sn_any */
		tetris::device::id the_server (11,0,0/*,tetris::device::id::sn_any*/);
//		tetris::device::id the_server ("1:0:0:0");
//		tetris::device::id the_server1 ("1:0:0:0");
		Json::Value params, result;
		struct timeval t1, t2;
		gettimeofday (&t1, NULL);
		while (!should_exit) {
			hp->async_call (the_server, 0, "hello", params,
							string (), tetris::never_timeout,
							MAKE_DEVICE_ASYNC_CALLBACK (client_device, async_callback));
			if (true)
				sleep (1);
		}
		gettimeofday (&t2, NULL);
		double delta = t2.tv_sec-t1.tv_sec+(t2.tv_usec-t1.tv_usec)/1000000.f;
		TETRIS_DEBUG("%ld calls made, cps: %0.1f", count, count / delta);
	}
	DECLARE_DEVICE_ASYNC_CALLBACK (async_callback)
	{
		if (finished == true) {

			/* async callbacks also need to deal with locking */
			boost::mutex::scoped_lock lock (count_mutex);
			TETRIS_DEBUG("client device callback response = %s",result["response"].toStyledString().c_str());

			count++;
		}
	}
private:
	bool debug_mode;
	bool should_exit;
	boost::thread* the_caller;
	long count;
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
		if (function == 0)
			ADD_DEVICE_METHOD (methods, server_device, hello);
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
		TETRIS_DEBUG("%ld requests serviced, rps: %0.1f\n", count, count / delta);
	}
private:
	DECLARE_DEVICE_METHOD (hello)
	{
		/* device methods are called by the bus cocurrently, so
		 * they have to deal with locking while accessing shared variables */
		boost::mutex::scoped_lock lock (count_mutex);
		result["response"]=Json::Value(" cpu xxxxx ");
		TETRIS_DEBUG("server_device :accept method= %s",method.c_str());
		count++;
//		/*test get_devices*/
//		//tetris::device::id fliter(-2,-2,-2,-2);//self
//		tetris::device::id fliter(-1,-1,-1,-1);
//		vector <device::id> devices;
//		vector <device::id>* devices_ptr= &devices;
//
//		int c=hp->get_devices (fliter,devices_ptr);
//		if(c>0){
//			cout<<"c="<<c<<endl;
//			for(vector <device::id> ::iterator it=devices.begin();it!=devices.end();++it){
//				tetris::device::id devid=*it;
//				cout<<"		fliter device id="<<devid.u.s.service<<" "<<devid.u.s.instance<<" "<<devid.u.s.role<<" "<<devid.u.s.sn<<endl;}
//		}
		return tetris::device::method_finished;
	}
private:
	long count;
	boost::mutex count_mutex;
	struct timeval startup_time, shutdown_time;

};



#if 1

int should_exit = 0;
void sighandler (int /*signum*/)	{ should_exit = 1; }


int main (int argc, char *argv []){

	string tetris_root;
	string c_id_str;
	int un_useP;
	X509* ca_cert;
	tetris::device * device_ptr;
	string cacert_file,device_conf_file,user_conf_file,device_cert_file,device_key_file;
	Json::Value bus_options;


	/* process the command line options ,and store in bus_options,c_id_str is set by --device*/
	if(global::parseOptions(argc,argv,bus_options,tetris_root,c_id_str,un_useP)!=0){
		TETRIS_ERROR("Error: Cannot parse options.");
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
		TETRIS_ERROR("Error: Cannot load the root cert.");
		return -1;
	}
	signal (SIGINT, sighandler);

	Json::Value configure;
	int seconds;
	if(!global::loadJsonConf(configure)){
		TETRIS_ERROR("Error: configure file load failed.");
		return -1;
	}
	if(configure.isMember("DEVICE_ATTACH_INTERVAL") && configure["DEVICE_ATTACH_INTERVAL"].isInt()){
		seconds=configure["DEVICE_ATTACH_INTERVAL"].asInt();
	}else{
		TETRIS_ERROR("Error: DEVICE_ATTACH_INTERVAL is not set,please set in configuration file.");
		return -1;
	}


	/*step1:init bus*/
	/* create a new bus and initialize it */
	tetris::bus* bus = tetris::new_bus ("distributed");
	if (bus == NULL){
		TETRIS_ERROR("Error: Cannot create the bus.");
		return -1;
	}
	if (!bus->init (ca_cert, bus_options, NULL)) {
		TETRIS_ERROR("Error: Cannot init the bus.");
		return -1;
	}
	/* start up the bus.
	 * devices can be attached/detached anytime after the bus started */

	if (!bus->startup ()) {
		TETRIS_ERROR("Error: Cannot start the bus.");
		return -1;
	}
	/*attach register device which for generateing registering call and save certificate*/
	tetris_service_register rd(ca_cert,device_conf_file,device_cert_file,device_key_file,c_id_str);
	tetris::bus::cd_login rd_cd = {"whatever", "anything"};
	bus->attach_device (&rd, rd_cd);
	TETRIS_DEBUG("Attach tetris_service_register.");

	device_str_map attached_func_devices,attached_user_devices;
	/* wait for the 'Ctrl+C' */
	while (!should_exit){

		device_str_set func_devices,user_devices;
		device_str_set ready_func_devices,detach_func_devices;
		device_cd_map devices_cd;

		global::scanFileName(device_conf_file,func_devices);
		global::scanFileName(user_conf_file,user_devices);
		if(func_devices.find(c_id_str)==func_devices.end()
				&& user_devices.find(c_id_str)!=user_devices.end() ){
			TETRIS_DEBUG("Can not attach device# %s, because no configuration file.",c_id_str.c_str());
			break;
		}

		if(attached_func_devices.find(c_id_str)!=attached_func_devices.end()){
			TETRIS_DEBUG("Already attach device# %s ",c_id_str.c_str());
			sleep (seconds);
			continue;
		}
		if(attached_user_devices.find(c_id_str)!=attached_user_devices.end()){
			TETRIS_DEBUG("Already attach device# %s",c_id_str.c_str());
			sleep (seconds);
			continue;
		}


		/*load all device's cert and private key
		 * cert and key file name like:2:0:0:0*/
		global::loadDeviceCd(device_cert_file,device_key_file,devices_cd);

		/*find out ready devices and attach*/
		if(func_devices.find(c_id_str)!=func_devices.end()){

			vector <string> vs;
			boost::split (vs, c_id_str, boost::is_any_of (":"));
			int size=vs.size();
			TETRIS_DEBUG("vs size == %d", size);
			if (vs.size () != 5){
				TETRIS_ERROR("Error: Device# %s name is not illegal.",c_id_str.c_str());
				break;
			}
			if (vs.size () == 5 && vs[4]!="c"){
				TETRIS_ERROR("Error: Device# %s name is not illegal.must have :c.",c_id_str.c_str());
				break;
			}
			string id_str =vs[0]+":"+vs[1]+":"+vs[2]+":"+vs[3];

			if(devices_cd.find(id_str)!=devices_cd.end()){
				tetris::device::id id(id_str);
				tetris::bus::cd_certificate cd = devices_cd[id_str];
				switch(id.u.s.service){
				case global::service_resource_manager:
					TETRIS_DEBUG("attach tetris_resource_manager.");
					device_ptr = new tetris_resource_manager;
					bus->attach_device(device_ptr,cd);
					attached_func_devices[c_id_str]=device_ptr;break;
				case global::service_test:
					TETRIS_DEBUG("attach service_test. ");
					device_ptr = new server_device;
					bus->attach_device(device_ptr,cd);
					attached_func_devices[c_id_str]=device_ptr;break;
				case global::service_test_11:
					TETRIS_DEBUG("attach service_test_11");
					device_ptr = new server_device;
					bus->attach_device(device_ptr,cd);
					attached_func_devices[c_id_str]=device_ptr;break;
				default:	break;
				}//switch
			}

		}else{
			string device_name,user_name;
			vector <string> vs;
			boost::split (vs, c_id_str, boost::is_any_of (":"));
			if (vs.size () != 3){
				TETRIS_ERROR("Error: User device# %s name is not illegal.",c_id_str.c_str());
				break;
			}
			if (vs.size () == 3 && vs[2]!="c"){
				TETRIS_ERROR("Error: User device# %s name is not illegal.must have :c.",c_id_str.c_str());
				break;
			}
			string id_str = vs[0]+":"+vs[1];
			device_name = vs[0];
			user_name= vs[1];
			tetris::bus::cd_login cd = {vs[1],"anything"};
			int user_device_id;
			stringstream ss(device_name);
			ss>>user_device_id;
			if(ss.fail()){
				TETRIS_ERROR("Error: transfer device id: %s from string to int.",id_str.c_str());
				continue;
			}
			switch(user_device_id){
				case global::client_test:	{
					TETRIS_DEBUG("attach client_test.");
					device_ptr = new client_device ;bus->attach_device(device_ptr,cd);
					attached_user_devices[c_id_str]=device_ptr;break;}
				case 0:{
				//device_ptr = new tetris_service_gateway ;bus->attach_device(device_ptr,cd);
					break;
				}
				default:	break;
			}//switch
		}
		sleep (seconds);
	}

	TETRIS_DEBUG("tetrisc terminating......");
	/* this will detach/shutdown all devices automatically */
	tetris::delete_bus (bus);
	return 0;
}
#endif
