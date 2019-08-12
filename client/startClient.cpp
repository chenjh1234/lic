
#include "startClient.h"
#include <QDebug>
#include <tetris/init_ssl.hpp>
#include <tetris/global_setting.hpp>

X509 *ca_cert;
EVP_PKEY *ca_private_key;
#if 0
bool generate_credential_data(string cn, X509 **pcert, EVP_PKEY **ppkey)
{
   static long serial = 1;
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
#endif
//========================================================================
StartClient::StartClient()
{
    setPortal(0);
    QString agent ;
     bool debug_mode = false;
     getPortal();
     client = NULL;
     //qDebug() << "portal ,agent = " << isPortal() << getenv(ENV_LIC_AGENT);

     if (!isPortal())  
     {
         agent = getenv(ENV_LIC_AGENT);
         if (!agent .isEmpty()) 
         {
             cout << "Error !! We can only connect licAgent from protal\n";
             //return;
             exit(1);
         }
         initDev(); 
     }
      else
      {
          //qDebug() << "start new Noserver";
          client  = new NoServer(debug_mode);  
           //qDebug() << "after new Noserver";
          client->setPortal(_portal,_portalIp,_portalPort);
           //qDebug() << "after set portal";
      }
}

StartClient::~StartClient()
{
    if (!isPortal())  
        closeDev(); 
  
}
void StartClient::setPortal(int idx)
{
    _portal = idx;
}
bool StartClient::isPortal()
{
    return _portal;
}
void StartClient::getPortal()
{
    int idx;
          
         _portalPort= getenv(ENV_PORTAL_PORT);
        if (_portalPort.isEmpty())  _portalPort= PORTAL_PORT;

        _portalIp = getenv(ENV_PORTAL_IP);

        if (_portalIp.isEmpty())  
            setPortal(0);      
        else 
        {
            idx = _portalIp.lastIndexOf(":");
            _portalIp = _portalIp.mid(0,idx);
            setPortal(1);
        }
}
int StartClient::closeDev()
{
     printf("\nterminating......\n");
     tetris::delete_bus(bus);

}

 
int StartClient::initDev()
{
       /* parse the options */
   int o;
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
   int _argc = 1;
   char * _argv[1];

// parse options:
   /* process the command line options ,and store in bus_options,c_id_str is set by --device*/
   if (global::parseOptions(_argc, _argv, bus_options, tetris_root, c_id_str, un_useP) != 0)
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

   /* load the root certificate/private key */
   //string a1="/home/geoeast/lg/NGC/chen/tetris/src/third/ca/cacert.pem";
   //string a1 = "/home/cjh/tetris/tetris/src/third/ca/cacert.pem";
   string a1 = cacert_file;
   if (!(ca_cert = tetris::pki::load_cert(a1)))
   {
      printf("failed to load the root cert\n");
      exit(-1);
   }

   // attach_um = 0;
   //  attach_server = 0;
 
   LFileDate fd;
   //unicast_port = fd.getFreePort(PORT_DEFAULT,PORT_LEN);

   /* create a new bus and initialize it */
   //tetris::bus* bus = tetris::new_bus ("distributed");
   bus = tetris::new_bus("distributed");

   if (bus == NULL) exit(-1);

   if (unicast_port != 0) bus_options["unicast_port"] = (unsigned int)unicast_port;
   bus_options["enhanced_security"] = enhanced_security;
   bus_options["debug_level"] = debug_mode ? "debug" : "critical";

   while(1)
   {
      unicast_port = fd.getFreePort(PORT_DEFAULT,PORT_LEN);
      if (unicast_port > 0) bus_options["unicast_port"] = (unsigned int)unicast_port;
      else
      {
          cout << "port error = " << unicast_port <<endl;
          exit(1);
      }
       cout << "bus init begin \n";
      if (!bus->init (ca_cert, bus_options, NULL)) 
      {
                   printf ("cannot init the bus\n");
                   exit (-1);
      }
       cout << "bus init ok \n";
      if (bus->startup())
      {
          //qDebug() << "port used  = " << unicast_port;  
          break;
      }  
   }
   /* start up the bus.
    * devices can be attached/detached anytime after the bus started */
#if 0
   if (!bus->startup())
   {
      printf("cannot start the bus\n");
      exit(-1);
   }
 #endif
   cout << "bus startup ok port = " <<  unicast_port << debug_mode << endl;
   //um  = new user_manager_device(um_cert, um_key);
   client  = new NoServer(debug_mode);
   cout << "start attach" << endl;
       /* the client device can also be identified by a certificate :) */
      tetris::bus::cd_login client_cd = { "whatever", "anything" };
      bus->attach_device(client, client_cd);
   cout << "client attached" << endl;

   return 0;
}
 
