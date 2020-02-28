#include "url_client.h"
#include "web_ssl.hpp"
#include <iostream>

namespace Web 
{
    std::atomic<std::uint64_t> Url_client::use_index_ {0};
	Url_client::Url_client(Scheme_type type,CERT_VER act,int retries,int timeout):type_(type),act_(act),
			retries_(retries),timeout_(timeout)
	{
        if(0 == use_index_.load()){
		    init();
	    }
        use_index_ ++;
    }

	Url_client::~Url_client()
	{
		free_resources();

        use_index_ --;
        if(0 == use_index_.load())
		    unInit();
	}

	bool Url_client:: post(const std::string &url,sp_httpmap header,const std::string &bodydata,http_callback sucess,http_callback fail)
	{
		free_resources();

		func_code ret = parse_url(url);
		if(func_code_OK != ret)
			return false;
		
		std::string crt;
		ret = do_openssl(crt);
		if(func_code_OK != ret)
			return false;
		failed_ = fail;
		sucessed_ = sucess;
		ret = do_web_request(header,bodydata);
		if(func_code_OK != ret)
			return false;

		return true;
	}
	
	bool Url_client::get(const std::string &url,sp_httpmap header,http_callback sucess,http_callback fail)
	{
		free_resources();

		func_code ret = parse_url(url);
		if(func_code_OK != ret)
			return false;
		
		std::string crt;
		ret = do_openssl(crt);
		if(func_code_OK != ret)
			return false;
		failed_ = fail;
		sucessed_ = sucess;
		std::string bodydata;
		ret = do_web_request(header,bodydata);
		if(func_code_OK != ret)
			return false;

		return true;
	}
	

	func_code Url_client::parse_url(const std::string &url)
	{
		http_uri_ = evhttp_uri_parse(url.c_str());
		if (http_uri_ == NULL) {
			err("malformed url");
			return func_code_evhttp_uri_parse;
		}

		scheme_ = evhttp_uri_get_scheme(http_uri_);
		if (scheme_ == NULL || (strcasecmp(scheme_, "https") != 0 &&
							   strcasecmp(scheme_, "http") != 0)) {
			err("url must be http or https");
			return func_code_no_scheme;
		}

		const char *host = evhttp_uri_get_host(http_uri_);
		if (NULL == host) {
			err("url must have a host");
			return func_code_no_host;
		}
		
		host_ = host;

		port_ = evhttp_uri_get_port(http_uri_);
		if (port_ == -1) {
			port_ = (strcasecmp(scheme_, "http") == 0) ? 80 : 443;
		}

		const char *path = evhttp_uri_get_path(http_uri_);
		if (strlen(path) == 0) {
			path = "/";
		}

		const char *query = evhttp_uri_get_query(http_uri_);
		if (query == NULL) {
			uri_ = path;
		} else {
			uri_ = path + std::string("?") + query;
		}
		return func_code_OK;
	}
	
	func_code Url_client::do_openssl(const std::string &crt)
	{
		/* This isn't strictly necessary... OpenSSL performs RAND_poll
		 * automatically on first use of random number generator. */
		int r = RAND_poll();
		if (r == 0) {
			err("RAND_poll failed");
			return func_code_rand_poll;
		}

		/* Create a new OpenSSL context */
		ssl_ctx_ = SSL_CTX_new(SSLv23_method());
		if (!ssl_ctx_) {
			err("SSL_CTX_new failed");
			return func_code_ssl_ctx_new;
		}

		if (crt.empty()) {
			X509_STORE *store;
			/* Attempt to use the system's trusted root certificates. */
			store = SSL_CTX_get_cert_store(ssl_ctx_);
			#ifdef _WIN32
					if (add_cert_for_store(store, "CA") < 0 ||
						add_cert_for_store(store, "AuthRoot") < 0 ||
						add_cert_for_store(store, "ROOT") < 0) {
						return func_code_add_cert;
					}
			#else // _WIN32
					if (X509_STORE_set_default_paths(store) != 1) {
						err("X509_STORE_set_default_paths failed");
						return func_code_store_cert;
					}
			#endif // _WIN32
		} else {
			if (SSL_CTX_load_verify_locations(ssl_ctx_, crt.c_str(), NULL) != 1) {
				err("SSL_CTX_load_verify_locations");
				return func_code_verify_cert;
			}
		}
		/* Ask OpenSSL to verify the server certificate.  Note that this
		 * does NOT include verifying that the hostname is correct.
		 * So, by itself, this means anyone with any legitimate
		 * CA-issued certificate for any website, can impersonate any
		 * other website in the world.  This is not good.  See "The
		 * Most Dangerous Code in the World" article at
		 * https://crypto.stanford.edu/~dabo/pubs/abstracts/ssl-client-bugs.html
		 */
		SSL_CTX_set_verify(ssl_ctx_, SSL_VERIFY_PEER, NULL);
		/* This is how we solve the problem mentioned in the previous
		 * comment.  We "wrap" OpenSSL's validation routine in our
		 * own routine, which also validates the hostname by calling
		 * the code provided by iSECPartners.  Note that even though
		 * the "Everything You've Always Wanted to Know About
		 * Certificate Validation With OpenSSL (But Were Afraid to
		 * Ask)" paper from iSECPartners says very explicitly not to
		 * call SSL_CTX_set_cert_verify_callback (at the bottom of
		 * page 2), what we're doing here is safe because our
		 * cert_verify_callback() calls X509_verify_cert(), which is
		 * OpenSSL's built-in routine which would have been called if
		 * we hadn't set the callback.  Therefore, we're just
		 * "wrapping" OpenSSL's routine, not replacing it. */
		SSL_CTX_set_cert_verify_callback(ssl_ctx_, cert_verify_callback,
						  (void *)this);
		return func_code_OK;
	}
	
	func_code Url_client::do_web_request(sp_httpmap header,const std::string  &bodydata)
	{
		// Create event base
		base_ = event_base_new();
		if (!base_) {
			err("event_base_new failed");
			return func_code_event_base_new;
		}

		// Create OpenSSL bufferevent and stack evhttp on top of it
		ssl_ = SSL_new(ssl_ctx_);
		if (ssl_ == NULL) {
			err("SSL_new failed");
			return func_code_ssl_new;
		}

		#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
		// Set hostname for SNI extension
		SSL_set_tlsext_host_name(ssl_, host_.c_str());
		#endif
		

		if (strcasecmp(scheme_, "http") == 0) {
			bev_ = bufferevent_socket_new(base_, -1, BEV_OPT_CLOSE_ON_FREE);
		} else {
			type_ = HTTPS;
			bev_ = bufferevent_openssl_socket_new(base_, -1, ssl_,
				BUFFEREVENT_SSL_CONNECTING,
				BEV_OPT_CLOSE_ON_FREE|BEV_OPT_DEFER_CALLBACKS);
		}

		if (bev_ == NULL) {
			err("bufferevent_openssl_socket_new failed");
			return func_code_bufferevent_new;
		}

		bufferevent_openssl_set_allow_dirty_shutdown(bev_, 1);

		// For simplicity, we let DNS resolution block. Everything else should be
		// asynchronous though.
		evcon_ = evhttp_connection_base_bufferevent_new(base_, NULL, bev_,host_.c_str(), port_);
		if (evcon_ == NULL) {
			err("evhttp_connection_base_bufferevent_new failed");
			return func_code_evhttp_connection;
		}

		if (retries_ > 0) {
			evhttp_connection_set_retries(evcon_, retries_);
		}
		if (timeout_ >= 0) {
			evhttp_connection_set_timeout(evcon_, timeout_);
		}

		// Fire off the request
		struct evhttp_request *req = evhttp_request_new(http_request_done, this);
		if (req == NULL) {
			err("evhttp_request_new failed");
			return func_code_evhttp_request_new;
		}

		struct evkeyvalq *output_headers = evhttp_request_get_output_headers(req);
		evhttp_add_header(output_headers, "Host", host_.c_str());
		evhttp_add_header(output_headers, "Connection", "close");
		if(header){
			for(auto it:*header)
			{
				evhttp_add_header(output_headers, it.first.c_str(), it.second.c_str());
			}
		}

		if (!bodydata.empty()) {
			struct evbuffer *output_buffer = evhttp_request_get_output_buffer(req);
			evbuffer_add(output_buffer, bodydata.c_str(), bodydata.length());
			std::string head_content = std::to_string(bodydata.length());
			evhttp_add_header(output_headers, "Content-Length", head_content.c_str());
		}

		int ret = evhttp_make_request(evcon_, req, bodydata.empty() ? EVHTTP_REQ_GET:EVHTTP_REQ_POST, uri_.c_str());
		if (ret != 0) {
			err("evhttp_make_request failed");
			return func_code_evhttp_make_request;
		}

		event_base_dispatch(base_);
		return func_code_OK;
	}

	func_code Url_client::init()
	{
        return Web_ssl::get_instance().web_ssl_init();
	}

	void Url_client::unInit()
	{
        return Web_ssl::get_instance().web_ssl_uninit();
	}	
	
	
#ifdef	_WIN32
	int	Url_client::add_cert_for_store(X509_STORE *store, std::string name)
	{
		HCERTSTORE sys_store = NULL;
		PCCERT_CONTEXT ctx = NULL;
		int r = 0;

		sys_store = CertOpenSystemStore(0, name.c_str());
		if (!sys_store) {
			err("failed to open system certificate store");
			return -1;
		}
		while ((ctx = CertEnumCertificatesInStore(sys_store, ctx))) {
			X509 *x509 = d2i_X509(NULL, (unsigned char const **)&ctx->pbCertEncoded,
				ctx->cbCertEncoded);
			if (x509) {
				X509_STORE_add_cert(store, x509);
				X509_free(x509);
			} else {
				r = -1;
				err("d2i_X509 failed");
				break;
			}
		}
		CertCloseStore(sys_store, 0);
		return r;
	}
#endif
	
	void Url_client::err(const std::string msg)
	{
		fputs(msg.c_str(), stderr);
	}

	void Url_client::free_resources()
	{
		if (evcon_){
			evhttp_connection_free(evcon_);
			evcon_ = nullptr;
		}
		if (http_uri_){
			evhttp_uri_free(http_uri_);
			http_uri_ = nullptr;
		}
		if (base_){
			event_base_free(base_);
			base_  = nullptr;
		}

		if (ssl_ctx_){
			SSL_CTX_free(ssl_ctx_);
			ssl_ctx_ = nullptr;
		}
		if (type_ == HTTP && ssl_){
			SSL_free(ssl_);
			ssl_ = nullptr;
		}
	}
	
	int Url_client::cert_verify_callback(X509_STORE_CTX *x509_ctx, void *arg)
	{
		char cert_str[256];
		Web::Url_client *client = static_cast<Url_client *>(arg);
		std::string res_str {"X509_verify_cert failed"};
		HostnameValidationResult res = Error;

		/* This is the function that OpenSSL would call if we hadn't called
		 * SSL_CTX_set_cert_verify_callback().  Therefore, we are "wrapping"
		 * the default functionality, rather than replacing it. */
		int ok_so_far = 0;

		X509 *server_cert = NULL;

		if (client->act_) {
			return 1;
		}

		ok_so_far = X509_verify_cert(x509_ctx);

		server_cert = X509_STORE_CTX_get_current_cert(x509_ctx);

		if (ok_so_far) {
			res = validate_hostname(client->host_.c_str(), server_cert);
		}

		X509_NAME_oneline(X509_get_subject_name (server_cert),
				  cert_str, sizeof (cert_str));

		if (res == MatchFound) {
			return 1;
		} else {
			return 0;
		}
	}
	
	void Url_client::http_request_done(struct evhttp_request *req, void *args)
	{
		sp_response sp_res = std::make_shared<Http_response>();
		Url_client *pthis = static_cast<Url_client *>(args);
		if (!req || !evhttp_request_get_response_code(req)) {
			/* If req is NULL, it means an error occurred, but
			 * sadly we are mostly left guessing what the error
			 * might have been.  We'll do our best... */
			struct bufferevent *bev = (struct bufferevent *) pthis->bev_;
			unsigned long oslerr;
			int printed_err = 0;
			int errcode = EVUTIL_SOCKET_ERROR();
			fprintf(stderr, "some request failed - no idea which one though!\n");
			/* Print out the OpenSSL error queue that libevent
			 * squirreled away for us, if any. */
			while ((oslerr = bufferevent_get_openssl_error(bev))) {
				char buffer[256];
				ERR_error_string_n(oslerr, buffer, sizeof(buffer));
				sp_res->code = Web::status_failed;
				sp_res->msg += buffer;
				printed_err = 1;
			}
			/* If the OpenSSL error queue was empty, maybe it was a
			 * socket error; let's try printing that. */
			if (! printed_err)
				sp_res->code = Web::status_failed;
				sp_res->msg += evutil_socket_error_to_string(errcode);
				sp_res->msg += std::to_string(errcode);
			
			if(pthis->failed_)
				pthis->failed_(sp_res);
			
			return;
		}

		sp_res->code = evhttp_request_get_response_code(req);
		int len = evbuffer_get_length(evhttp_request_get_input_buffer(req));
		char *read_buf = new char[len];
		int nread = evbuffer_remove(evhttp_request_get_input_buffer(req),read_buf, len);
		if(nread != len){
			sp_res->code = Web::status_read_wrong;
			if(pthis->failed_)
				pthis->failed_(sp_res);
		}
		else {
			sp_res->msg.assign(read_buf,nread);
			if(pthis->sucessed_)
				pthis->sucessed_(sp_res);
		}
		delete [] read_buf;
	}	
}
