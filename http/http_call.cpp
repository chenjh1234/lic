#include "http_call.hpp"
#include "curl/curl.h"

#include <tetris/debug.hpp>

HttpClient::HttpClient(void)
{
	portalIP.clear();
	forkPid=-1;
	pPort=0;
	bPort=0;
}

HttpClient::HttpClient(const string &ip, const unsigned short &port) :
	portalIP(ip),
	pPort(port)
{
	forkPid=-1;
}
HttpClient::~HttpClient(void)
{
}

void HttpClient::stop(void){
	TETRIS_DEBUG("HttpClient::stop!");
	if(forkPid>0){
		TETRIS_DEBUG("stop Proxy!");
		kill(forkPid,SIGKILL);
	}
}
bool HttpClient::init(void){

	/*Pattern 1: one App : one Proxy*/
	if(portalIP.empty()){
		/*set all*/
		portalIP=getIP();//"10.10.162.80";//
		pPort=getUnusedPort(DEFAULT_PORTAL_PORT,DEFAULT_PORT_NUM);
		bPort=getUnusedPort(DEFAULT_BUS_PORT,DEFAULT_PORT_NUM);
		if(bPort==0 || pPort==0)
			return false;
		stringstream strStream;
		strStream << bPort;
		string sb = strStream.str();
		strStream.str("");
		strStream << pPort;
		string sp = strStream.str();
		ipPort=portalIP+":"+sp;
		/*start proxy*/
		/* OK, fork it! -----------------> */
		if ((forkPid = fork ()) == -1) {
			return false;
		}
		if (forkPid == 0) {
			int myPid = getpid();
			TETRIS_DEBUG("PID1 value: %d",myPid);
			vector <string> args;
			string arg="--c";
			args.push_back(arg);
			args.push_back(sb);

			arg="--portal_port";
			args.push_back(arg);
			args.push_back(sp);

			string portalPath="/home/lxd/mytetris/mytetirs/build/src/tetris/portal";
			int argc = args.size()+2;
			char ** argv = (char **) malloc (sizeof (char*)*argc);
			argv [0] = strdup(portalPath.c_str());
			argv [argc-1] = NULL;
			for (int i=0;i<argc-2;i++)
				argv [i+1] = strdup (args[i].c_str());
			/* execute it! ------> */
			execv (portalPath.c_str(), argv);
		}
	}
	/*many apps : specific proxy*/
	else{
		stringstream strStream;
		strStream << pPort;
		string sp = strStream.str();
		ipPort=portalIP+":"+sp;
		/*just invoke proxy*/
		return true;
	}
	return true;
}

string HttpClient::getIP(void){
	string defaultIP="127.0.0.1";
	return defaultIP;
}

unsigned short HttpClient::getUnusedPort(const unsigned short& startNum,const unsigned short& number){
	unsigned short unPort=0;
	for(unsigned short port=startNum;port<startNum+number;port++){
		/* 1. create a stream socket */
		int unicast_socket = socket (AF_INET, SOCK_STREAM, 0);
		if (unicast_socket == -1) {
			continue;
		}
		/* 2. enable the REUSEADDR option */
		int enabled = 1;
		if (setsockopt (unicast_socket, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof (int)) < 0) {
	err:	close (unicast_socket);
			unicast_socket = -1;
			continue;
		}
		/* 3. bind the server address */
		struct sockaddr_in sa;
		memset (&sa, 0, sizeof (sa));
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = htonl (INADDR_ANY);
		sa.sin_port = htons (port);
		if (bind (unicast_socket, (struct sockaddr *) &sa, sizeof (sa)) < 0) {
			TETRIS_ERROR("Port: %u is already in use!", port);
			goto err;
		}else{
			close (unicast_socket);
			unPort=port;
			TETRIS_ERROR("Port: %u is unused!", port);
			break;
		}
	}
	return unPort;
}


bool HttpClient::Call(const std::string & serviceName, const std::string & method, const Json::Value& params, Json::Value& result)
{

	/*different services have different construct method
	 * 1.direct device, such as job manager
	 * 2.proxy device, such as resource manager*/
	string busMethod;
	Json::Value busParams;
	string url;
	Json::Value url_params;
	string portalIP_PORT=ipPort;
	if(serviceName.compare("job")==0){
		busMethod=method;
		busParams=params;
		url=portalIP_PORT+"/job/call";
	}
	else if(serviceName.compare("rm")==0){
		busMethod="call";/*for proxy device, only provide one bus method "call"*/
		busParams["url"]=method;/*detail tag--'url' referred device 'call' implement*/
		busParams["url_params"]=params;
		url=portalIP_PORT+"/rm/call";
        }
                // add_cjh
      else if(serviceName.compare("lm")==0){
		busMethod="call";/*for proxy device, only provide one bus method "call"*/
		busParams["url"]=method;/*detail tag--'url' referred device 'call' implement*/
		busParams["url_params"]=params;
		url=portalIP_PORT+"/lm/call";
      }
	else
		return false;

	url_params["method"]=busMethod;
	url_params["params"]=busParams;

	string str_response;
	TETRIS_DEBUG("URL: %s", url.c_str());
	TETRIS_DEBUG("url_params= %s", url_params.toStyledString().c_str());
	Json::FastWriter writer;
	string url_params_string=writer.write(url_params);

	struct timeval t1, t2;
	gettimeofday (&t1, NULL);

       //   int res1=Get(url, str_response);
        //TETRIS_DEBUG("Get ret = %d\n", res1);

	int res=Post(url,url_params_string,str_response);
        TETRIS_DEBUG("post ret = %d\n", res);
      

	gettimeofday (&t2, NULL);
	double delta = t2.tv_sec-t1.tv_sec+(t2.tv_usec-t1.tv_usec)/1000000.f;
	TETRIS_DEBUG("post time: %lf", delta);

	//return 0,if Post ok.
	if(res!=0)
		return false;

	TETRIS_DEBUG("reader.parse(str_response,response)): %s", str_response.c_str());
	Json::Reader reader;
	Json::Value response,tmp_response;
	if(!reader.parse(str_response,response)){
		return false;
	}
	result=response;
	return true;
}

static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)
{
	if(itype == CURLINFO_TEXT)
	{
		//TETRIS_DEBUG("[TEXT]%s", pData);
	}
	else if(itype == CURLINFO_HEADER_IN)
	{
		TETRIS_DEBUG("[HEADER_IN]%s", pData);
	}
	else if(itype == CURLINFO_HEADER_OUT)
	{
		TETRIS_DEBUG("[HEADER_OUT]%s", pData);
	}
	else if(itype == CURLINFO_DATA_IN)
	{
		TETRIS_DEBUG("[DATA_IN]%s", pData);
	}
	else if(itype == CURLINFO_DATA_OUT)
	{
		TETRIS_DEBUG("[DATA_OUT]%s", pData);
	}
	return 0;
}

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if( NULL == str || NULL == buffer )
	{
		return -1;
	}

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
	return nmemb;
}

int HttpClient::Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
        cout <<"strURL =" << strUrl << endl;
        cout << "strPost = " << strPost << endl;
        cout << " inPOST init = = " << curl <<endl;
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	curl_slist *plist = curl_slist_append(NULL,"Content-Type:application/json;charset=UTF-8");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 2);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/curlposttest.cookie");
	res = curl_easy_perform(curl);
        cout << "CURLE_OK = " << CURLE_OK << endl;
        cout << "res = " << res << endl;

	if (res != CURLE_OK)
	{
		TETRIS_ERROR("res not ok");
		switch(res)
		{
			case CURLE_UNSUPPORTED_PROTOCOL:
				TETRIS_ERROR("不支持的协议,由URL的头部指定");
			case CURLE_COULDNT_CONNECT:
				TETRIS_ERROR("不能连接到remote主机或者代理");
			case CURLE_HTTP_RETURNED_ERROR:
				TETRIS_ERROR("http返回错误");
			case CURLE_READ_ERROR:
				TETRIS_ERROR("读本地文件错误");
			default:
				TETRIS_ERROR("返回值: %d",res);
		}
		return -1;
	}
	curl_easy_cleanup(curl);
	return res;
}

int HttpClient::Get(const std::string & strUrl, std::string & strResponse)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);

	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

int HttpClient::Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	if(NULL == pCaPath)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

int HttpClient::Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	if(NULL == pCaPath)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}
