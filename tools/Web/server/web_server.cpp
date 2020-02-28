#include "web_server.h"
#include <thread>


namespace Web
{
    Web_server::Web_server(Scheme_type type,int port,int nthread,int timeout):type_(type),
    port_(port),nthread_(nthread),timeout_(timeout)
    {
        if(HTTPS == type_)
            Web_ssl::get_instance().web_ssl_init();
    }

    Web_server::~Web_server()
    {
        if(HTTPS == type_)
            Web_ssl::get_instance().web_ssl_uninit();
    }

    bool Web_server::start(http_callback callback)
    {
        func_code ret;
        if(HTTPS == type_){
            ret = do_openssl();
            if(func_code_OK != ret){
		err("do_openssl failed");
                return false;
	    }
        }
        callback_ = callback;        
        ret = do_server();
        if(func_code_OK != ret){
	    err("do_server failed");
            return false;
	}

        return true;
    }

    void Web_server::err(const std::string msg)
    {
        fputs(msg.c_str(),stderr);
    }

    func_code Web_server::do_openssl()
    {
        ctx_ = SSL_CTX_new (SSLv23_server_method ());
        SSL_CTX_set_options (ctx_,
                             SSL_OP_SINGLE_DH_USE |
                             SSL_OP_SINGLE_ECDH_USE |
                             SSL_OP_NO_SSLv2);
        EC_KEY *ecdh = EC_KEY_new_by_curve_name (NID_X9_62_prime256v1);
        if (! ecdh){
            err("EC_KEY_new_by_curve_name");
            return func_code_ec_key;
        }
        if (1 != SSL_CTX_set_tmp_ecdh (ctx_, ecdh)){
            err("SSL_CTX_set_tmp_ecdh");
            return func_code_set_ec_key;
        }

        const char *certificate_chain = "server-certificate-chain.pem";
        const char *private_key = "server-private-key.pem";
        if (1 != SSL_CTX_use_certificate_chain_file (ctx_, certificate_chain)){
            err("SSL_CTX_use_certificate_chain_file");
            return func_code_ssl_cert;
        }

        if (1 != SSL_CTX_use_PrivateKey_file (ctx_, private_key, SSL_FILETYPE_PEM)){
            err("SSL_CTX_use_PrivateKey_file");
            return func_code_ssl_use_key;
        }

        if (1 != SSL_CTX_check_private_key (ctx_)){
            err("SSL_CTX_check_private_key");
            return func_code_ssl_check_key;
        }

        return func_code_OK;
    }

    func_code Web_server::socket_bind(uint16_t port,int &fd)
    {
        int ret = -1;

        fd = socket(AF_INET,SOCK_STREAM,0);
        if ( fd  < 0 ){
            err("socket failed");
            return func_code_http_bind_socket;
        }

        int32_t one = 1;
        ret = setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(char *)&one,sizeof(int32_t));
        if( ret < 0  ){
            err("setsockopt failed");
            return func_code_http_setsockopt;
        }

        struct sockaddr_in addr;
        memset(&addr,0,sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("0.0.0.0");
        addr.sin_port = htons(port);

        if(::bind(fd,(struct sockaddr *)&addr,sizeof(addr)) < 0){
            err("bind failed");
            return func_code_http_bind_socket;
        }
        
        if(listen(fd,10) < 0){
            err("listen failed");
            return func_code_http_listen;
        }

        int flags ;
        if((flags = fcntl(fd,F_GETFL,0)) < 0
           || fcntl(fd,F_SETFL,flags | O_NONBLOCK) < 0){
            err("fcntl failed");
            return func_code_http_fcntl;
        }

        return func_code_OK;
    }

    func_code Web_server::do_server()
    {
        func_code ret;
        int fd ;

        ret = socket_bind(port_,fd);
        if(ret != func_code_OK){
            err("socket_bind failed");
            return ret;
        }

        for(auto i = 0; i <= nthread_;i++){
            struct event_base *base = event_init();
            if(!base){
                err("event_init failed");
                return func_code_http_event_init;
            }

            struct evhttp *http = evhttp_new(base);
            if(!http){
                err("evhttp_new failed");
                return func_code_http_evhttp_new;
            }
            if(evhttp_accept_socket(http, fd) != 0){
                err("evhttp accept failed");
                return func_code_http_evhttp_accept;
            }

           if(HTTPS == type_)
                evhttp_set_bevcb (http, bevcb, ctx_);

            evhttp_set_timeout(http, timeout_);

            /* This is the callback that gets called when a request comes in. */
            evhttp_set_gencb (http, request_handle, this);
            std::thread t(&Web_server::thread_work,this,base);
            t.detach();
        }
	return ret;
    }

    void Web_server::thread_work(struct event_base *base)
    {
        event_base_dispatch (base);    
    }

    struct bufferevent* Web_server::bevcb (struct event_base *base, void *arg)
    {
        struct bufferevent* r;
        SSL_CTX *ctx = (SSL_CTX *) arg;
        r = bufferevent_openssl_socket_new (base,-1,SSL_new (ctx),
                                            BUFFEREVENT_SSL_ACCEPTING,
                                             BEV_OPT_CLOSE_ON_FREE);
        return r;
    }

    void Web_server::request_handle(struct evhttp_request *req, void *args)
    {
        Web_server *pthis = static_cast<Web_server *>(args);
        struct evbuffer *evb = NULL;
        const char *uri = evhttp_request_get_uri (req);
        struct evhttp_uri *decoded = NULL;

        if (evhttp_request_get_command (req) == EVHTTP_REQ_GET)
        {
            struct evkeyvalq * header = evhttp_request_get_input_headers(req);
            const char *key = evhttp_find_header(header,"abc");
            printf("url:%s %s\n",uri,key);
            struct evbuffer *buf = evbuffer_new();
            if (buf == NULL) return;
            evbuffer_add_printf(buf, "Requested: %s\n", uri);
            evhttp_send_reply(req, HTTP_OK, "OK", buf);
            return;
                                                                                            
        }

        /* We only handle POST requests. */
        if (evhttp_request_get_command (req) != EVHTTP_REQ_POST)
        { 
            evhttp_send_reply (req, 200, "OK", NULL);
          return;
        }

        decoded = evhttp_uri_parse (uri);
        if (! decoded)
        {
            printf ("It's not a good URI. Sending BADREQUEST\n");
            evhttp_send_error (req, HTTP_BADREQUEST, 0);
            return;
        }
        /* Decode the payload */
        struct evkeyvalq kv;
        memset (&kv, 0, sizeof (kv));
        struct evbuffer *buf = evhttp_request_get_input_buffer (req);
        evbuffer_add (buf, "", 1);    /* NUL-terminate the buffer */
        char *payload = (char *) evbuffer_pullup (buf, -1);
        if(payload[0] == '\0')
        {
             printf ("post args nothings\n");
             evhttp_send_error (req, HTTP_BADREQUEST, 0);
             return;
        }
        if (0 != evhttp_parse_query_str (payload, &kv))
        { 
             printf ("Malformed payload. Sending BADREQUEST %s\n",payload);
             evhttp_send_error (req, HTTP_BADREQUEST, 0);
             return;
        }

        /*
        char *peer_addr;
        ev_uint16_t peer_port;
        struct evhttp_connection *con = evhttp_request_get_connection (req);
        evhttp_connection_get_peer (con, &peer_addr, &peer_port);
        */
        sp_response sp_res = std::make_shared<Http_response>();
        sp_res->code = Web::status_read_wrong;
        pthis->callback_(sp_res);        
        char response[256]; 
        evutil_snprintf(response,sizeof(response),"%s","hello,welcome come in");
        evhttp_clear_headers (&kv);
        evb = evbuffer_new ();
        evhttp_add_header (evhttp_request_get_output_headers (req),
                                                  "Content-Type", "application/x-yaml");
        evbuffer_add (evb, response, strlen (response));
        evhttp_send_reply (req, 200, "OK", evb);

        if (decoded)
            evhttp_uri_free (decoded);

        if (evb)
            evbuffer_free (evb);
    }
}
