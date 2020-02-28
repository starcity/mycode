#include "web_server.h"


web_server::web_server()
{
	phttp = new http_server;
	phttps = new https_server;
}

web_server::~web_server()
{
}

int32_t web_server::start()
{
	shared_ptr<http_server> http_ptr(new http_server);
	shared_ptr<https_server> https_ptr(new https_server);

	if(nullptr == http_ptr)
}

int32_t web_server::get_config()
{

}


