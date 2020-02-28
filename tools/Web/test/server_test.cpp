#include "web_server.h"
#include "web_ssl.hpp"
#include <iostream>



int main()
{
    Web::Web_server s(Web::Web_server::HTTP,8080,3,3);
    Web::http_callback sucess = [](Web::sp_response spres){std::cout<<__LINE__<<""<<spres->msg<<std::endl;};
    if(!s.start(sucess)){
        std::cout<<"server failed"<<std::endl;
        return -1;
    }

    std::string msg;
    while(1){
        std::cin>>msg;
        if(msg == "q")
            break;
    }
}
