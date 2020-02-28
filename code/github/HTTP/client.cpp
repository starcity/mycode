#include "http_client.h"

namespace Web{
    namespace http_client{
        http_client::http_client()
        {
            _base = event_base_new();
            if(!_base){
                LOG_ERROR("{}:{} create event base failed",__FILE__,__LINE__);
                return;
            }

            _dnsBase =  evdns_base_new(base, 1);
           if(!_dnsBase){
                LOG_ERROR("{}:{} create dns event base failed",__FILE__,__LINE__);
                return;
           }
        }

        ~http_client::http_client()
        {
            if(_dnsBase)
                evdns_base_free(_dnsBase,0);

            if(_base)
                event_base_free(_base);
        }

        bool http_client::get(std::string &url,Key_value &header,Sucess_callback sucess,Fail_callback fail)
        {
            struct evhttp_request* request = evhttp_request_new(NULL,NULL);
             evhttp_request_set_chunked_cb(request, ck); 
             evhttp_request_set_error_cb(request, RemoteRequestErrorCallback);   
        }

    }
}
