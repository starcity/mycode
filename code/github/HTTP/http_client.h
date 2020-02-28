#pragma once
#include <string>
#include <map>
#include <memory>
#include <functional>
#include "event2/http.h"
#include "event2/http_struct.h"
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/dns.h"
#include "event2/thread.h"
#include "spdlog/slog.h"


namespace Web 
{
    namespace http_client{
        
        enum class status_code{
            FAILED,
        };

        struct Response{
            status_code  code;
            std::string  msg;
        };

        using Key_value = std::map<std::string,std::string>;
        using Sucess_callback = std::function<void(std::string)>;
        using Fail_callback = std::function<void(std::shared_ptr<Response>)>;


        class http_client
        {
        public:
            http_client();
            ~http_client(); 

            bool get(std::string &url,Key_value &header,Sucess_callback sucess,Fail_callback fail);
            bool post(std::string &url,Key_value &header,Sucess_callback sucess,Fail_callback fail);

        private:
            struct event_base* _base {nullptr};
            struct evdns_base* _dnsBase {nullptr};
        };
    }
}
