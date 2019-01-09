
#include <http_call.hpp>
#include <unistd.h>
#include <stdlib.h>
//#include <tetris/debug.hpp>
#include "comDefs.h"
//#define TETRIS_DEBUG printf

int main (int argc, char *argv []){

	Json::Value jv2,jv1,r1,r2;

	

	#if 1
        //case 1:
	 //* multi App: one Proxy
	string ip="localhost";//"10.10.162.80";
	unsigned short port=18080;
	HttpClient hc(ip,port);
	#endif

	/*case 2:
	 * one App : one Proxy*/
	//HttpClient hc;
	/*step 1 : initial*/
        TETRIS_DEBUG("ip,: %s ",  ip.c_str());
        TETRIS_DEBUG("port,: %d ",  port);
	hc.init();
	//sleep(1);
        TETRIS_DEBUG("init ok\n");
	/*
	 * bool HttpClient::Call(const std::string & serviceName, const std::string & method, const Json::Value& params, Json::Value& result)
	 * serviceName={job,rm,um,sm}, now have four choices
	 * method, for job manager is bus Method, for rm ,is ip+port+method,
	 * like (http://182.92.111.53:8080)/BGPPlatform/asset/getAssetConfiguration
	 * */
        jv1[REPORT_PARAM] = REPORT_PARAM_PACKAGE;
	/*step 2 : invoke*/
        bool b;
	b = hc.Call("lm",CMD_REPORT,jv1,r1);
        TETRIS_DEBUG("call ret = %d\n",b);



	//jv2["cid"]=5000;
	//hc.Call("rm","BGPPlatform/asset/getAssetConfiguration",jv2,r2);

	/*step 3 : if want to close App, stop*/
	hc.stop();

	TETRIS_DEBUG("r1 result: %s", r1.toStyledString().c_str());
	//TETRIS_DEBUG("r2 result: %s", r2.toStyledString().c_str());

}


