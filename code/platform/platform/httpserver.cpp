#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <unistd.h>
#include <event.h>
#include <evhttp.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "httpserver.h"
#include "common.h"
#include <stdint.h>
#include "json/json.h"
#include "log/Log.h"
#include "businessmanagement.h"
#include "cmd.h"
#include <yaml.h>


#define  CONFIG_FILE	"config.yaml"

httpServer::httpServer()
{  
	read_config();
}

httpServer::~httpServer()
{
}


void httpServer::read_config()
{
    FILE *fp = fopen(CONFIG_FILE,"r");
    yaml_parser_t parser;
    yaml_token_t  token;
    char name[50] = {0};

    if(!yaml_parser_initialize(&parser)){
    	WRITELOGINFO("Failed to initzlize parser");
        return ;
    }
    if(NULL == fp){
    	WRITELOGINFO("Failed to open config.yaml");
        return ;
    }
    
    yaml_parser_set_input_file(&parser,fp);

    do {
	yaml_parser_scan(&parser,&token);
	
	if(token.type == YAML_SCALAR_TOKEN){
		if(strcmp(name,"ip") == 0){
			httpd_option_listen = (char *)token.data.scalar.value;
		}
		if(strcmp(name,"port") == 0){
			char *p = (char *)token.data.scalar.value;
		    httpd_option_port = atoi(p);
		}
		if(strcmp(name,"timeout") == 0){
			char *p = (char *)token.data.scalar.value;
			httpd_option_timeout = atoi(p);
		}
		if(strcmp(name,"thread") == 0){
			char *p = (char *)token.data.scalar.value;
			httpd_option_thread = atoi(p);;
		}
		else {
	        	char *p = (char *)token.data.scalar.value;
        	  strncpy(name,p,sizeof(name) -1);
		}
	}
	if(token.type != YAML_STREAM_END_TOKEN)
		yaml_token_delete(&token);
    } while(token.type != YAML_STREAM_END_TOKEN);
    
    yaml_token_delete(&token);
    yaml_parser_delete(&parser);
    fclose(fp);
}

int32_t httpServer::start()
{
    if(initialize() < 0)
        return CODE_RETURN_ERROR_INITIALIZE;
    httpserver_start(httpd_option_port,httpd_option_thread,10);

    return CODE_RETURN_SUCESSED;
}

int32_t httpServer::initialize()
{
    m_business = new Business();//业务启动
    m_business->init();

    return CODE_RETURN_SUCESSED;
}

int32_t httpServer::cmdDistribution(struct evhttp_request *req)
{
    const char *uri;
    string reInfo;
    Json::Value		root;
    char withDealUrl[1024] = {0};//处理url中的空格
    int total = 0;

    uri = evhttp_request_uri(req);

    //decoded uri
    char *decoded_uri;
    decoded_uri = evhttp_decode_uri(uri);
    total = strlen(decoded_uri);
    memcpy(withDealUrl,decoded_uri,total);
    for(int i =0;i < total;i++)
        if(withDealUrl[i] == ' ')withDealUrl[i] = '+';

    WRITELOGINFO("decoded_uri:%s",decoded_uri);

    struct evkeyvalq params;
    evhttp_parse_query(withDealUrl, &params);

    m_business->doPlatform(params,reInfo);

    httpSend(req,reInfo);
    evhttp_clear_headers(&params);//释放内存   如果不调用会造成内存泄露
    free(decoded_uri);

    return CODE_RETURN_SUCESSED;
}

int32_t httpServer::httpSend(struct evhttp_request *req,string info)
{
    evhttp_add_header(req->output_headers, "Server", MYHTTPD_VERSION);
    evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");

    struct evbuffer *buf;
    buf = evbuffer_new();
    evbuffer_add_printf(buf, "%s\n", info.c_str());
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);

    return CODE_RETURN_SUCESSED;
}

int32_t httpServer::httpserver_bindsocket(int32_t port, int32_t backlog)
{
    int32_t r;
    int32_t nfd;
    nfd = socket(AF_INET, SOCK_STREAM, 0);
    if (nfd < 0) return -1;

    int32_t one = 1;
    r = setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int32_t));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(httpd_option_listen.c_str());//INADDR_ANY;
    addr.sin_port = htons(port);

    r = ::bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
    if (r < 0) return -1;
    r = listen(nfd, backlog);
    if (r < 0) return -1;

    int flags;
    if ((flags = fcntl(nfd, F_GETFL, 0)) < 0
        || fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0)
        return -1;

    return nfd;
}

int32_t httpServer::httpserver_start(int32_t port, int32_t nthreads,int32_t backlog)
{
    int r, i;
    int nfd = httpserver_bindsocket(port, backlog);
    if (nfd < 0) return -1;
    pthread_t ths[nthreads];
    for (i = 0; i < nthreads; i++) {
        struct event_base *base = event_init();
        if (base == NULL) return -1;
        struct evhttp *httpd = evhttp_new(base);
        if (httpd == NULL) return -1;
        r = evhttp_accept_socket(httpd, nfd);
        if (r != 0) return -1;
        evhttp_set_timeout(httpd, httpd_option_timeout);
        evhttp_set_gencb(httpd, httpserver_GenericHandler, this);
        r = pthread_create(&ths[i], NULL, httpserver_Dispatch, base);
        if (r != 0) return -1;
    }
    for (i = 0; i < nthreads; i++) {
        pthread_join(ths[i], NULL);
    }
    return CODE_RETURN_SUCESSED;
}

void* httpServer::httpserver_Dispatch(void *arg)
{
    event_base_dispatch((struct event_base*)arg);
    return NULL;
}

void httpServer::httpserver_GenericHandler(struct evhttp_request *req, void *arg)
{
    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
        gClog.WriteLog(CLog::LL_ERROR,"%s:%d:Non-GET request. Sending BADREQUEST",__FILE__,__LINE__);
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    httpServer *http_server = (httpServer *)arg;
    http_server->cmdDistribution(req);

    return;
}

