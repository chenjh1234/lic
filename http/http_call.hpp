#ifndef __HTTP_CURL_H__
#define __HTTP_CURL_H__

#include <string>
#include <iostream>
#include <json/json.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <signal.h>
//#include <tetris/debug.hpp>

using namespace std;

#define DEFAULT_PORTAL_PORT	18080
#define DEFAULT_BUS_PORT		20000
#define DEFAULT_PORT_NUM		100

class HttpClient
{
public:
	HttpClient(const string &ip, const unsigned short &port);
	HttpClient(void);//default local ip and port
	~HttpClient(void);

private:
	/**
 * 	* @brief HTTP POST请求
 * 		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
 * 			* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
 * 				* @param strResponse 输出参数,返回的内容
 * 					* @return 返回是否Post成功
 * 						*/
	int Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);

	/**
 * 	* @brief HTTP GET请求
 * 		* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
 * 			* @param strResponse 输出参数,返回的内容
 * 				* @return 返回是否Post成功
 * 					*/
	int Get(const std::string & strUrl, std::string & strResponse);

	/**
 * 	* @brief HTTPS POST请求,无证书版本
 * 		* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
 * 			* @param strPost 输入参数,使用如下格式para1=val1¶2=val2&…
 * 				* @param strResponse 输出参数,返回的内容
 * 					* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
 * 						* @return 返回是否Post成功
 * 							*/
	int Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath = NULL);

	/**
 * 	* @brief HTTPS GET请求,无证书版本
 * 		* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
 * 			* @param strResponse 输出参数,返回的内容
 * 				* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
 * 					* @return 返回是否Post成功
 * 						*/
	int Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath = NULL);
	unsigned short getUnusedPort(const unsigned short& startNum,const unsigned short& number);
	string getIP(void);

public:
	/*
	 * init work, to manage portalIP , portal port and bus port
	 * */
	bool init(void);
	void stop(void);
	/*
	 * success - true
	 * fail	  - false
	 * */
	bool Call(const std::string & serviceName, const std::string & method, const Json::Value& params, Json::Value& result);

private:
	string portalIP;
	unsigned short pPort,bPort;//set in init(void)
	string ipPort;//ip:port
	int forkPid;

};

#endif

