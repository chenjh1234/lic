/*
 * simApp.cpp
 *simulate Geo application, call bus method by using 'httpcall' library
 *
 *  Created on: 2017年10月20日
 *      Author: lxd
 */
#include <http_call.hpp>
#include <unistd.h>
#include <stdlib.h>
#include <tetris/debug.hpp>

int main (int argc, char *argv []){

	Json::Value jv2,jv1,r1,r2;
	jv1["type"]=23;

	/*case 1:
	 * multi App: one Proxy
	string ip="127.0.0.1";//"10.10.162.80";
	unsigned short port=40080;
	HttpClient hc(ip,port);
	*/

	/*case 2:
	 * one App : one Proxy*/
	HttpClient hc;
	/*step 1 : initial*/
	hc.init();
	sleep(10);
	/*
	 * bool HttpClient::Call(const std::string & serviceName, const std::string & method, const Json::Value& params, Json::Value& result)
	 * serviceName={job,rm,um,sm}, now have four choices
	 * method, for job manager is bus Method, for rm ,is ip+port+method,
	 * like (http://182.92.111.53:8080)/BGPPlatform/asset/getAssetConfiguration
	 * */

	/*step 2 : invoke*/
	hc.Call("job","Get",jv1,r1);

	jv2["cid"]=5000;
	hc.Call("rm","BGPPlatform/asset/getAssetConfiguration",jv2,r2);

	/*step 3 : if want to close App, stop*/
	hc.stop();

	TETRIS_DEBUG("r1 result: %s", r1.toStyledString().c_str());
	TETRIS_DEBUG("r2 result: %s", r2.toStyledString().c_str());

}


