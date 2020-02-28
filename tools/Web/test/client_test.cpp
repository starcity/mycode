#include "url_client.h"
#include <iostream>
#include <unistd.h>
#include <thread>


int main()
{
	Web::Url_client c(Web::Url_client::HTTPS,Web::Url_client::IGNORE);
	std::string url("http://127.0.0.1:8080/?some=2323");
	Web::sp_httpmap header = std::make_shared<Web::httpmap>();
	(*header)["name"]  = "guiyou";
	Web::http_callback failed = [](Web::sp_response spres){std::cout<<__LINE__<<""<<spres->msg<<std::endl;};
	Web::http_callback sucess = [](Web::sp_response spres){std::cout<<__LINE__<<""<<spres->msg<<std::endl;};

    std::thread t([]{
	    Web::Url_client c(Web::Url_client::HTTPS,Web::Url_client::IGNORE);
    	std::string url("http://127.0.0.1:8080/?some=2323");
    	Web::sp_httpmap header = std::make_shared<Web::httpmap>();
    	(*header)["name"]  = "guiyou";
        Web::http_callback failed = [](Web::sp_response spres){std::cout<<__LINE__<<""<<spres->msg<<std::endl;};
    	Web::http_callback sucess = [](Web::sp_response spres){std::cout<<__LINE__<<""<<spres->msg<<std::endl;};
      	for(auto i = 0;i < 1000;i++){
	       	c.get(url,header,sucess,failed);
	        std::string msg("i love you girl!");
	        if(!c.post(url,header,msg,sucess,failed))
                std::cout<<"thread "<<i<<std::endl;
	    }

    });
    t.detach();

	sleep(10);

	for(auto i = 0;i < 1000;i++){
		c.get(url,header,sucess,failed);
		std::string msg("i love you girl!");
	    if(!c.post(url,header,msg,sucess,failed))
                std::cout<<"main "<<i<<std::endl;
	}
}
