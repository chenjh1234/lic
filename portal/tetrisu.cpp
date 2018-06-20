/*
 * tetrisu.cpp
 *
 *  Created on: 2016年6月27日
 *      Author: lxd
 */
#include <tetris/device.hpp>
#include <distributed_bus.hpp>
#include <tetris_um.hpp>
#include <tetris/global_setting.hpp>
#include <unistd.h>
#include <tetris/init_ssl.hpp>


int should_exit = 0;
void sighandler (int /*signum*/)	{ should_exit = 1; }

int main (int argc, char *argv []){


	X509* ca_cert,* um_cert;
	EVP_PKEY* ca_private_key,* um_key;
	string cacert_file,ca_private_key_file,um_cert_file,um_key_file,um_conf_file;
	string tetris_root,user_id_str,un_use;
	int un_useP;
	Json::Value bus_options,configure;

	string pidfile = "/var/run/tetrisu";
	if (!global::singleton (pidfile.c_str())) {
		TETRIS_ERROR("Error: tetrisu already running.");
		return -1;
	}

	if(!global::loadJsonConf(configure)){
		TETRIS_ERROR("Error: configure file load failed.");
		return false;
	}
	if(configure.isMember("device_id_um") && configure["device_id_um"].isString()){
		user_id_str=configure["device_id_um"].asString();
	}else{
		TETRIS_ERROR("Error: device_id_um is not set,please set in configuration file.");
		return false;
	}


	/* process the command line options ,and store in bus_options*/
	if(global::parseOptions(argc,argv,bus_options,tetris_root,un_use,un_useP)!=0){
		TETRIS_ERROR("Error: Cannot parser options.");
		return -1;
	}
	cacert_file = tetris_root+CACERT_FILE;
	ca_private_key_file = tetris_root+CA_PRIVATE_KEY_FILE;
	um_conf_file = tetris_root+DEVICE_CONF_FILE+"/"+user_id_str;
	um_cert_file = tetris_root+DEVICE_CERT_FILE+"/"+user_id_str;
	um_key_file = tetris_root+DEVICE_KEY_FILE+"/"+user_id_str;

	/*check if exists user manager configure file*/
	if((access(um_conf_file.c_str(),F_OK)!=0)){
		TETRIS_ERROR("Error: Cannot load user manager configure file,not exist.");
		return -1;
	}
	/* init the openssl library */
	OpenSSL_add_all_algorithms ();
	ERR_load_crypto_strings ();
	init_ssl_locks ();

	/* load the root certificate/private key */
	if (!(ca_cert = tetris::pki::load_cert (cacert_file))) {
		TETRIS_ERROR("Error: Cannot load the root certificate.");
		return -1;
	}
	if (!(ca_private_key = tetris::pki::load_private_key (ca_private_key_file))) {
		TETRIS_ERROR("Error: Cannot load the root key.");
		return -1;
	}
	tetris_user_manager um (ca_cert,ca_private_key);

	if((access(um_cert_file.c_str(),F_OK)!=0) || (access(um_key_file.c_str(),F_OK)!=0)){
		/**/
		um.generate_credential_data(user_id_str,&um_cert,&um_key);
		tetris::pki::save_cert(um_cert,um_cert_file);
		tetris::pki::save_private_key(um_key,um_key_file);
	}

	if (!(um_cert = tetris::pki::load_cert (um_cert_file))) {
		TETRIS_ERROR("Error: Cannot load the user manager certificate.");
		return -1;
	}

	if (!(um_key = tetris::pki::load_private_key (um_key_file))) {
		TETRIS_ERROR("Error: Cannot load the user manager key.");
		return -1;
	}

	if ((!tetris::pki::verify(ca_cert,um_cert)) || (!tetris::pki::samepair(um_cert,um_key))){
		TETRIS_ERROR("Error: certificate and key are not a pair.");
		return -1;
	}
	if(!um.set_credential_data(um_cert,um_key)){
		TETRIS_ERROR("Error: Cannot set the user manager's credential data.");
		return -1;
	}

	signal (SIGINT, sighandler);
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
	/* start up the bus.*/
	if (!bus->startup ()) {
		TETRIS_ERROR("Error: Cannot start the bus.");
		return -1;
	}
	/*attach user manager device*/

	tetris::bus::cd_certificate um_cd (um_cert, um_key);
	bus->attach_device (&um, um_cd);


	/* wait for the 'Ctrl+C' */
	while (!should_exit){
		sleep (1);
	}
	TETRIS_DEBUG("tetrisu terminating......");
	/* this will detach/shutdown all devices automatically */
	tetris::delete_bus (bus);

	return 0;

}


