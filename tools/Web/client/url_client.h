#pragma once
// Get rid of OSX 10.7 and greater deprecation warnings.
#if defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdint>
#include <atomic>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define snprintf _snprintf
#define strcasecmp _stricmp
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include "openssl_hostname_validation.h"
#include "header.h"


namespace Web 
{
	class Url_client{
		public:
			enum Scheme_type {HTTP,HTTPS};	
			enum CERT_VER 
			{
				VERIFICATION = 0,
				IGNORE = 1,
			};
			Url_client(Scheme_type type = HTTP,CERT_VER act = IGNORE,int retries = 3,int timeout = 3);
//			Url_client() = delete;
			~Url_client();
			
			bool post(const std::string &url,sp_httpmap header,const std::string &bodydata,http_callback sucess,http_callback fail);
			bool get(const std::string &url,sp_httpmap header,http_callback sucess,http_callback fail);
			
		private:
			func_code parse_url(const std::string &url);
			func_code do_openssl(const std::string &crt);	
			func_code do_web_request(sp_httpmap header,const std::string &bodydata);
			func_code init();
			void unInit();
			#ifdef _WIN32
			int	add_cert_for_store(X509_STORE *store, std::string name);	
			#endif
			void 	free_resources();
			
			
			static int cert_verify_callback(X509_STORE_CTX *x509_ctx, void *arg);
			static void http_request_done(struct evhttp_request *req, void *ctx);
	    	static void err(const std::string msg);
			
		private:
            static std::atomic<std::uint64_t> use_index_;
			http_callback			failed_ {nullptr};
			http_callback			sucessed_ {nullptr};
			struct event_base 		*base_ {nullptr};
			struct evhttp_connection 	*evcon_ {nullptr};
			struct bufferevent 		*bev_ {nullptr};
			struct evhttp_uri 		*http_uri_ {nullptr};
			SSL_CTX 			*ssl_ctx_ {nullptr};
			SSL 				*ssl_ {nullptr};
			std::string 			uri_;
			std::string				host_;
			int						port_;
			const char *			scheme_ {nullptr};
			CERT_VER				act_	{IGNORE};
			int						retries_;
			int 					timeout_;//seconds
			Scheme_type			type_ {HTTP};
	};
}

