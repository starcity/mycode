#ifndef     _WEB_SERVER_H_
#define		_WEB_SERVER_H_
#include "http_server.h"
#include "https_server.h"

extern CLog gclog;


class web_server
{
	public:
		web_server();
	   ~web_server();

	   int32_t start();


	private:
	   int32_t get_config();


	private:
	   http_server  *phttp;
	   https_server *phttps;
};


#endif
