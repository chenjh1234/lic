#ifndef UMSERVER_HPP__ 
#define UMSERVER_HPP__

#include <stdio.h>
#include <sys/time.h>
#include <tetris/device.hpp>

#include <set>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <uuid/uuid.h>
#include <tetris/bus.hpp>
 
#include <dirent.h>

/* lxd  */
/* the simulated user manager device */
class user_manager_device : public tetris::device
{
public:
	/* we need a certificate and a private key to sign the tokens */
	user_manager_device (X509* cert, EVP_PKEY* pkey)
	{
		um_cert = X509_dup (cert);
		um_key = tetris::pki::evp_pkey_dup (pkey);
	}
	~user_manager_device ()
	{
		X509_free (um_cert);
		EVP_PKEY_free (um_key);
	}
private:
	unsigned get_methods (const unsigned& function, method_map& methods)
	{
		/* we have only one method.. */
		if (function == 0)
			ADD_DEVICE_METHOD (methods, user_manager_device, authenticate);
		return methods.size ();
	}
private:
	DECLARE_DEVICE_METHOD (authenticate)
	{
		/* extract the public key to seal the token data */
		string pubkey_str = params [2].asString ();
		EVP_PKEY* pubkey = tetris::pki::decode_public_key (pubkey_str);
		if (pubkey == NULL) {
			printf ("cannot extract the sealing key\n");
			return tetris::device::method_failed;
		}

		/* generate the token */
		Json::Value token_data;
		string token_str, token_key_str;
		if (!tetris::pki::generate_token (um_cert, um_key, token_data, token_str, token_key_str)) {
			printf ("error generating the token\n");
			EVP_PKEY_free (pubkey);
			return tetris::device::method_failed;
		}
		/* seal the token data */
		string ek1, ek2, sealed_token_str, sealed_token_key_str;
		if (!tetris::pki::seal (token_str, sealed_token_str, ek1, pubkey) ||
			!tetris::pki::seal (token_key_str, sealed_token_key_str, ek2, pubkey)) {
			printf ("error sealing the token/key\n");
			EVP_PKEY_free (pubkey);
			return tetris::device::method_failed;
		}
		/* generate the response.
		 * jsoncpp cannot encode binary strings correctly,
		 * so we have to do a base64 encoding before saving them in the response */
		result [0] = true;
		result [1] = tetris::pki::base64_encode (ek1);
		result [2] = tetris::pki::base64_encode (sealed_token_str);
		result [3] = tetris::pki::base64_encode (ek2);
		result [4] = tetris::pki::base64_encode (sealed_token_key_str);
		EVP_PKEY_free (pubkey);
		return tetris::device::method_finished;
	}
private:
	X509* um_cert;
	EVP_PKEY* um_key;
};

#endif

