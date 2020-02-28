#pragma once
#include <functional>
#include <memory>
#include <string>
#include <map>

namespace Web 
{
	enum func_code 
	{
		func_code_OK = 0,
		func_code_window_startup_wrong = -0xA000,
		func_code_evhttp_uri_parse = -0xA010,
		func_code_no_scheme = -0xA011,
		func_code_no_host = -0xA012,
		func_code_event_base_new   = -0xA013,
		func_code_bufferevent_new  = -0xA014,
		func_code_evhttp_connection = -0xA015,
		func_code_evhttp_request_new = -0xA016,
		func_code_evhttp_make_request = 0xA017,
		func_code_rand_poll = -0xA100,
		func_code_ssl_ctx_new = -0xA101,
		func_code_add_cert	  = -0xA102,
		func_code_store_cert  = -0xA103,
		func_code_verify_cert = -0xA104,
		func_code_ssl_new	  = -0xA105,
		func_code_ec_key	  = -0xB000,
		func_code_set_ec_key  = -0xB001,
        func_code_ssl_cert    = -0xB002,
        func_code_ssl_use_key = 0xB003, 
        func_code_ssl_check_key = 0xB004, 
        func_code_evhttp_new  = 0xB005,
        func_code_http_bind_socket = 0xB006,
        func_code_http_socket = 0xB007,
        func_code_http_setsockopt = 0xB008,
        func_code_http_listen = 0xB009,
        func_code_http_fcntl = 0xB00A,
        func_code_http_event_init = 0xB00B,
        func_code_http_evhttp_new = 0xB00C,
        func_code_http_evhttp_accept = 0xB00D,
	};
	
	enum  /*status_code*/ 
	{
		status_ok = 200,
		status_failed = 100,
		status_read_wrong = 101,
	};
	
	struct Http_response
	{
		int          code;
		std::string  msg;
	};
	
	using httpmap = std::map<std::string,std::string>;
	using sp_httpmap = std::shared_ptr<httpmap>;
	using sp_response = std::shared_ptr<Http_response>;
	using http_callback = std::function<void(sp_response res)>; 
}
