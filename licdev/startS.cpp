#include <stdio.h>
#include <sys/time.h>
#include <tetris/device.hpp>
#include "distributed_bus.hpp"
#include "util.hpp"
#include <dirent.h>
#include <tetris/init_ssl.hpp>
#include <tetris/global_setting.hpp>
//debug setting
#include "umServer.hpp"
#include "licServer.hpp"
#include "licAgent.hpp"


int should_exit = 0;
void sighandler(int /*signum*/)
{ should_exit = 1; }

#define TETRISD_DEBUG_ENABLED			true

#define debug_x(fmt,args...) \
	if(TETRISD_DEBUG_ENABLED)\
		printf(fmt,args);

X509 *ca_cert;
EVP_PKEY *ca_private_key;


bool get_credential_data(string cn, X509 **pcert, EVP_PKEY **ppkey)
{
   static long serial = 1;
   cout << "server = " << cn << endl;
   X509_REQ *req = tetris::pki::generate_cert_req(ca_cert, cn, ppkey);
   if (req == NULL)
   {
      printf("error generating the cert req\n");
      return false;
   }
   string req_str = tetris::pki::encode_cert_req(req);
   X509_REQ_free(req);

   if (!(*pcert = tetris::pki::generate_cert(ca_cert, ca_private_key, req_str, serial++)))
   {
      printf("error generating the cert\n");
      EVP_PKEY_free(*ppkey);
      return false;
   }
   if (!tetris::pki::verify(ca_cert, *pcert))
   {
      printf("cannot verify the newly generated cert\n");
      X509_free(*pcert);
      EVP_PKEY_free(*ppkey);
      return false;
   }
   return true;
}
#if 0 //for ger cert on site
int startServer(int argc, char *argv[])
{

   /* parse the options */
   int o;
   int attach_um;
   int attach_server ;

   unsigned short unicast_port = 0;
   bool enhanced_security = false;
   bool debug_mode = false;

 
   string tetris_root;
   string c_id_str;
   int un_useP;
   //X509* ca_cert;
   tetris::device *device_ptr;
   string cacert_file, device_conf_file, user_conf_file, device_cert_file, device_key_file;
   Json::Value bus_options;
   un_useP = 0;
   c_id_str = "";

// parse options:
   /* process the command line options ,and store in bus_options,c_id_str is set by --device*/
   if (global::parseOptions(argc, argv, bus_options, tetris_root, c_id_str, un_useP) != 0)
   {
      TETRIS_ERROR("Error: Cannot parse options.");
      return -1;
   }
   string cakey_file;

   cacert_file = tetris_root + CACERT_FILE;
   cakey_file = tetris_root + CA_PRIVATE_KEY_FILE;

   device_conf_file = tetris_root + DEVICE_CONF_FILE;

   device_cert_file = tetris_root + DEVICE_CERT_FILE;
   device_key_file = tetris_root + DEVICE_KEY_FILE;

   user_conf_file = tetris_root + USER_CONF_FILE;

/* init the openssl library */
   OpenSSL_add_all_algorithms();
   ERR_load_crypto_strings();
   /* do this before making any other openssl calls */
   init_ssl_locks();

   cout << "ca_cert_file =" << cacert_file << " \n" << cakey_file << endl;
   if (!(ca_cert = tetris::pki::load_cert(cacert_file)))
   {
      printf("failed to load the root cert\n");
      exit(-1);
   }

   //if (!(ca_private_key = tetris::pki::load_private_key ("/home/geoeast/lg/NGC/chen/tetris/src/third/ca/cakey.pem"))) {
   if (!(ca_private_key = tetris::pki::load_private_key(cakey_file)))
   {
      printf("failed to load the root key\n");
      exit(-1);
   }

// generate the certificate/private key pairs
   X509 *um_cert;
   X509 * server_cert;
   EVP_PKEY *um_key;
   EVP_PKEY * server_key;

   string serverid, umid, agentid;
   int num;
//dfault:
   agentid = LIC_AGENT_ID;
   serverid = LIC_SERVER_ID;
   umid = LIC_UM_ID;
//server IDs:
   cout << "serverid,agentid =   " << serverid << agentid << endl;
   cout << "c_id_str,un_useP=" << c_id_str << ";" << un_useP << endl;

   //attach um if deviceID = 5:0:0:0

   attach_server = 1;
   attach_um = 0;
   //
   if (c_id_str.empty())
   {
      attach_um = 0;
   }
   else if (c_id_str == serverid)
   {
      attach_um = 1;
   }
   else
   {
      serverid = c_id_str;
      attach_server = 0;
   }
 // cds:
  	if (!get_credential_data (umid, &um_cert, &um_key)
	||!get_credential_data (serverid, &server_cert, &server_key))
		exit (-1);
 
   /* create a new bus and initialize it */
   tetris::bus *bus = tetris::new_bus("distributed");

   if (bus == NULL) exit(-1);
   //Json::Value bus_options;
   if (unicast_port != 0) bus_options["unicast_port"] = (unsigned int)unicast_port;
   bus_options["enhanced_security"] = enhanced_security;
   bus_options["debug_level"] = debug_mode ? "debug" : "critical";
   if (!bus->init(ca_cert, bus_options, NULL))
   {
      printf("cannot init the bus\n");
      exit(-1);
   }
   /* start up the bus.
    * devices can be attached/detached anytime after the bus started */
   signal(SIGINT, sighandler);
   cout << "bus startup  "   << endl;
   if (!bus->startup())
   {
      printf("cannot start the bus\n");
      exit(-1);
   }
   cout << "attach device: "   << endl;
   /* attach the devices */
   user_manager_device um(um_cert, um_key);
   LicAgent aserver;
   LicServer server;
 
   if (attach_um)
   {
      tetris::bus::cd_certificate um_cd(um_cert, um_key);
      bus->attach_device(&um, um_cd);
      cout << "um   attached " << endl;
   }
 
   if (attach_server)  // server
   {
     tetris::bus::cd_certificate server_cd(server_cert, server_key);
      bus->attach_device(&server, server_cd);
      cout << "server attached =" << serverid << endl;
   }
   else // agent
   {
      tetris::bus::cd_certificate aserver_cd(server_cert, server_key);
      bus->attach_device(&aserver,aserver_cd);
      cout << "agent attached =" << serverid << endl;
   }

 

   /* wait for the 'Ctrl+C' */
   while (!should_exit)
   {
      sleep(1);
      if (attach_server && server._down == 1)  break;
      if (!attach_server && aserver._down == 1)  break;
   }
   printf("\nterminating......\n");
   /* this will detach/shutdown all devices automatically */
   tetris::delete_bus(bus);

   return 0;
}
#endif
#if 1// for load and save 
int startServer(int argc, char *argv[])
{

   /* parse the options */
   int o;
   int attach_um;
   int attach_server ;

   unsigned short unicast_port = 0;
   bool enhanced_security = false;
   bool debug_mode = false;

 
   string tetris_root;
   string c_id_str;
   int un_useP;
   //X509* ca_cert;
   tetris::device *device_ptr;
   string cacert_file, device_conf_file, user_conf_file, device_cert_file, device_key_file;
   Json::Value bus_options;
   un_useP = 0;
   c_id_str = "";

// parse options:
   /* process the command line options ,and store in bus_options,c_id_str is set by --device*/
   if (global::parseOptions(argc, argv, bus_options, tetris_root, c_id_str, un_useP) != 0)
   {
      TETRIS_ERROR("Error: Cannot parse options.");
      return -1;
   }
   string cakey_file;

   cacert_file = tetris_root + CACERT_FILE;
   cakey_file = tetris_root + CA_PRIVATE_KEY_FILE;

   device_conf_file = tetris_root + DEVICE_CONF_FILE;

   device_cert_file = tetris_root + DEVICE_CERT_FILE;
   device_key_file = tetris_root + DEVICE_KEY_FILE;

   user_conf_file = tetris_root + USER_CONF_FILE;

/* init the openssl library */
   OpenSSL_add_all_algorithms();
   ERR_load_crypto_strings();
   /* do this before making any other openssl calls */
   init_ssl_locks();

//CA: loade:  cert,key:
// cacert_file = CA_CERT;
//cakey_file = CA_KEY;

   cout << "ca_cert_file =" << cacert_file << " \n" << cakey_file << endl;
   if (!(ca_cert = tetris::pki::load_cert(cacert_file)))
   {
      printf("failed to load the root cert\n");
      exit(-1);
   }

   //if (!(ca_private_key = tetris::pki::load_private_key ("/home/geoeast/lg/NGC/chen/tetris/src/third/ca/cakey.pem"))) {
   if (!(ca_private_key = tetris::pki::load_private_key(cakey_file)))
   {
      printf("failed to load the root key\n");
      exit(-1);
   }

// generate the certificate/private key pairs
   X509 *um_cert;
   X509 * server_cert,*server_cert1,*server_cert2;
   EVP_PKEY *um_key;
   EVP_PKEY * server_key,*server_key1,*server_key2;

   string serverid, umid, agentid;
   int num;
   //dfault:
   agentid = LIC_AGENT_ID;
   serverid = LIC_SERVER_ID;
   umid = LIC_UM_ID;
//server IDs:
   cout << "serverid,agentid =   " << serverid << agentid << endl;
   cout << "c_id_str,un_useP=" << c_id_str << ";" << un_useP << endl;

   //attach um if deviceID = 5:0:0:0

   attach_server = 1;
   attach_um = 0;
   if (c_id_str.empty())
   {
      attach_um = 0;
   }
   else if (c_id_str == serverid)
   {
      attach_um = 1;
   }
   else
   {
      serverid = c_id_str;
      attach_server = 0;
   }
    //attach_um = 1;
    //attach_server = 0;

   

   //num = loadDeviceCd(string &path_cert,string &path_key,device_cd_map & cd_map);
   device_cd_map   cd_map;

   num = global::loadDeviceCd(device_cert_file, device_key_file, cd_map);
   cout << "loaded cd = " << num;
 
   //um:
   if (attach_um)
   {
      if (cd_map.find(umid) == cd_map.end())// no this cd
      {
         if ((!get_credential_data(umid, &um_cert, &um_key)))
         {
            cout << "no um cd data  \n";
            exit(1);
         }
         else
         {
             qDebug() << " aaaaaaa";
            if (!tetris::pki::save_cert(um_cert, device_cert_file + "/" + umid))
               cout << "cannot save cert file " << device_cert_file + "/" + umid << endl;
            if (!tetris::pki::save_private_key(um_key, device_key_file + "/" + umid)) 
              cout << "cannot save key file " << device_key_file + "/" + umid<< endl;

            bool ok111 = tetris::pki::samepair(um_cert, um_key);
            cout << "um verify result====" << ok111 << endl;
         }
      }
   }
 
//server:
   if (cd_map.find(serverid) == cd_map.end())
   {
      if ((!get_credential_data(serverid, &server_cert, &server_key)))
      {
         cout << "no server cd data  \n";
         exit(1);
      }
      else
      {
         if (!tetris::pki::save_cert(server_cert, device_cert_file + "/" + serverid)) 
             cout << "cannot save cert file " << device_cert_file + "/" + serverid << endl;
          if (!tetris::pki::save_private_key(server_key, device_key_file + "/" + serverid)) 
             cout << "cannot save key file " << device_key_file + "/" + serverid << endl;
      }
   }


   /* create a new bus and initialize it */
   tetris::bus *bus = tetris::new_bus("distributed");

   if (bus == NULL) exit(-1);
   //Json::Value bus_options;
   if (unicast_port != 0) bus_options["unicast_port"] = (unsigned int)unicast_port;
   bus_options["enhanced_security"] = enhanced_security;
   bus_options["debug_level"] = debug_mode ? "debug" : "critical";
   if (!bus->init(ca_cert, bus_options, NULL))
   {
      printf("cannot init the bus\n");
      exit(-1);
   }
   /* start up the bus.
    * devices can be attached/detached anytime after the bus started */
   signal(SIGINT, sighandler);
   cout << "bus startup  "   << endl;
   if (!bus->startup())
   {
      printf("cannot start the bus\n");
      exit(-1);
   }
   cout << "attach device: "   << endl;
   /* attach the devices */
   LicAgent aserver;
   LicServer server;
qDebug() << "111";

    cd_map.clear();
    num = global::loadDeviceCd(device_cert_file, device_key_file, cd_map);
   cout << "loaded cds = " << num<<endl;
   cout << "attach device begin : mark un server = "   << attach_um <<" " <<attach_server<< endl;
  map< string, tetris::bus::cd_certificate>::iterator it;

   for  (it=cd_map.begin() ; it != cd_map.end(); it++) 
   {
        cout << it->first << endl; 
   }
  
   {
      
       /* load the root certificate/private key */
              if (!(um_cert = tetris::pki::load_cert (device_cert_file + "/" + umid))) {
                      TETRIS_ERROR("Error: Cannot load the root certificate.");
                      return -1;
              }
              if (!(um_key = tetris::pki::load_private_key (device_key_file + "/" + umid))) {
                      TETRIS_ERROR("Error: Cannot load the root key.");
                      return -1;
              }
   }
   user_manager_device um(um_cert, um_key);

    if (attach_um)
    {
        
      qDebug() << "um0";
      //tetris::bus::cd_certificate um_cd(um_cert, um_key);
      tetris::bus::cd_certificate um_cd= cd_map[umid];
      qDebug() << "um = " <<um_cert << um_key;
      bus->attach_device(&um, um_cd);
      cout << "um   attached " << endl;
   }
 
   if (attach_server)  // server
   {
      tetris::bus::cd_certificate server_cd = cd_map[serverid];

     // tetris::bus::cd_certificate server_cd(server_cert, server_key);
      //qDebug() << "server = " <<server_cert << server_key;
      bus->attach_device(&server, server_cd);
      cout << "server attached =" << serverid << endl;
   }
   else // agent
   {
        tetris::bus::cd_certificate aserver_cd = cd_map[serverid];
        qDebug() << "agent";
      bus->attach_device(&aserver,aserver_cd);
      cout << "server attached =" << serverid << endl;
   }

 

   /* wait for the 'Ctrl+C' */
   while (!should_exit)
   {
      sleep(1);
      if (attach_server && server._down == 1)  break;
      if (!attach_server && aserver._down == 1)  break;
   }
   printf("\nterminating......\n");
   /* this will detach/shutdown all devices automatically */
   tetris::delete_bus(bus);

   return 0;
}
#endif

