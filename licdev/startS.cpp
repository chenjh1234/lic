#include <stdio.h>
#include <sys/time.h>
#include <tetris/device.hpp>
#include "distributed_bus.hpp"
#include "util.hpp"
#include <dirent.h>
#include <tetris/init_ssl.hpp>
//debug setting
#include "umServer.hpp"
#include "licServer.hpp"
//#include "noServer.hpp"

int should_exit = 0;
void sighandler (int /*signum*/)
{ should_exit = 1; }

#define TETRISD_DEBUG_ENABLED			true

#define debug_x(fmt,args...) \
	if(TETRISD_DEBUG_ENABLED)\
		printf(fmt,args);

X509* ca_cert;
EVP_PKEY* ca_private_key;

bool generate_credential_data (string cn, X509** pcert, EVP_PKEY** ppkey)
{
	static long serial = 1;
        cout << "server = " << cn << endl;
	X509_REQ* req = tetris::pki::generate_cert_req (ca_cert, cn, ppkey);
	if (req == NULL) {
		printf ("error generating the cert req\n");
		return false;
	}
	string req_str = tetris::pki::encode_cert_req (req);
	X509_REQ_free (req);
	if (!(*pcert = tetris::pki::generate_cert (ca_cert, ca_private_key, req_str, serial++))) {
		printf ("error generating the cert\n");
		EVP_PKEY_free (*ppkey);
		return false;
	}
	if (!tetris::pki::verify (ca_cert, *pcert)) {
		printf ("cannot verify the newly generated cert\n");
		X509_free (*pcert);
		EVP_PKEY_free (*ppkey);
		return false;
	}
	return true;
}


int startServer (int argc, char* argv [])
{

	/* parse the options */
	int o;
	int attach_um = 0;
	int attach_server = 0;
	int attach_client = 0;
	unsigned short unicast_port = 0;
	bool enhanced_security = false;
	bool debug_mode = false;
	while ((o = getopt (argc, argv, "uscp:ed")) != -1) {
		switch (o) {
		case 'u': attach_um = 1; break;
		case 's': attach_server = 1; break;
		case 'c': attach_client = 1; break;
		case 'p': unicast_port = atoi (optarg); break;
		case 'e': enhanced_security = true; break;
		case 'd': debug_mode = true; break;
		default: break;
		}
	}


	/* init the openssl library */
	OpenSSL_add_all_algorithms ();
	ERR_load_crypto_strings ();

	/* do this before making any other openssl calls */
	init_ssl_locks();

	/* load the root certificate/private key */
	//string a1="/home/geoeast/lg/NGC/chen/tetris/src/third/ca/cacert.pem";
       // string a1="/home/cjh/tetris/tetris/src/third/ca/cacert.pem";
        string a1 = CA_CERT;
	if (!(ca_cert = tetris::pki::load_cert (a1))) {
		printf ("failed to load the root cert\n");
		exit (-1);
	}

	//if (!(ca_private_key = tetris::pki::load_private_key ("/home/geoeast/lg/NGC/chen/tetris/src/third/ca/cakey.pem"))) {
        if (!(ca_private_key = tetris::pki::load_private_key (CA_KEY))) {
		printf ("failed to load the root key\n");
		exit (-1);
	}

	/* generate the certificate/private key pairs */
	X509* um_cert;
	X509* server_cert,*server_cert1,*server_cert2;
	EVP_PKEY* um_key;
	EVP_PKEY* server_key,*server_key1,*server_key2;
        string serverid;
        serverid = LIC_SERVER_ID;

        cout<<"serverid =   " << serverid << "," << LIC_SERVER_ID<< endl;

	if ((!generate_credential_data ("1:0:0:0", &um_cert, &um_key))
	||!generate_credential_data (serverid, &server_cert, &server_key)
		|| !generate_credential_data ("3:0:0:0", &server_cert1, &server_key1))
//		|| !generate_credential_data ("4:0:0:0", &server_cert2, &server_key2))
		exit (-1);


//	tetris::pki::save_cert(server_cert,"/tmp/cert/11:0:0:0");
//	tetris::pki::save_private_key(server_key,"/tmp/key/11:0:0:0");

	cout<<"xxx"<<endl;
	bool ok111=tetris::pki::samepair(um_cert,um_key);
	cout<<"verify result===="<<ok111<<endl;
	string path="/tmp/cert";
	string path1="/tmp/key";
//	device_cd_map map;

//	bool ok =loadDeviceCd(path,path1,map);
//	cout<<"loadDeviceCd.size()="<<map.size()<<endl;

	/* create a new bus and initialize it */
	tetris::bus* bus = tetris::new_bus ("distributed");

	if (bus == NULL)
		exit (-1);
	Json::Value bus_options;
	if (unicast_port != 0)
		bus_options ["unicast_port"] = (unsigned int) unicast_port;
	bus_options ["enhanced_security"] = enhanced_security;
	bus_options ["debug_level"] = debug_mode?"debug":"critical";
	if (!bus->init (ca_cert, bus_options, NULL)) {
		printf ("cannot init the bus\n");
		exit (-1);
	}
	/* start up the bus.
	 * devices can be attached/detached anytime after the bus started */
	signal (SIGINT, sighandler);
	if (!bus->startup ()) {
		printf ("cannot start the bus\n");
		exit (-1);
	}
        cout<<"bus startup  "   << endl;
	/* attach the devices */
	user_manager_device um (um_cert, um_key);
	LicServer server;
//	tetris_service_portal sp;
	//NoServer client (debug_mode);
//tetris_job_manager_master jsfserver;

//	tetris::bus::cd_certificate server_cd1 (server_cert1,server_key1);// um_key);//server_key);
//	bus->attach_device (&jsfserver, server_cd1);
//	cout<<"attach jsfserver"<<endl;
        cout<<"start attach"<<endl;
        // false the two server = 1;
        attach_um = 1;
        attach_server = 1;
        attach_client = 0;


	if (attach_um) {
		tetris::bus::cd_certificate um_cd (um_cert, um_key);
		bus->attach_device (&um, um_cd);
		cout<<"attach um"<<endl;
	}
         cout<<"------------um attached"<<endl;
	if (attach_server) {
		tetris::bus::cd_certificate server_cd (server_cert,server_key);// um_key);//server_key);
		bus->attach_device (&server, server_cd);
		cout<<"attach ser"<<endl;
	}
        cout<<"---------------server attached id = " << serverid <<endl;
        #if 0
	if (attach_client) {
		/* the client device can also be identified by a certificate :) */
		tetris::bus::cd_login client_cd = {"whatever", "anything"};
		bus->attach_device (&client, client_cd);
		cout<<"attach cli"<<endl;
	}
        cout<<"--------------client attached"<<endl;
       #endif
	/* wait for the 'Ctrl+C' */
	while (!should_exit){
		sleep (1);
            if (server._down == 1)  break;
        }
	printf ("\nterminating......\n");
	/* this will detach/shutdown all devices automatically */
	tetris::delete_bus (bus);

	return 0;
}


