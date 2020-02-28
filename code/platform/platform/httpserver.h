#ifndef     _HTTP_SERVER_H_
#define     _HTTP_SERVER_H_

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <evhttp.h>
#include "common.h"
#include "taskinterface.h"



extern CLog	gClog;


using namespace std;


class httpServer
{   
public:
	httpServer();
	~httpServer();


public:
    int32_t start();

private://
    static void  httpserver_GenericHandler(struct evhttp_request *req, void *arg);
    static void* httpserver_Dispatch(void *arg);
    static void *httpDealWith(void *arg);
	static void *threadDealWith(void *arg);

private:
    //http server
    int32_t httpserver_start(int32_t port,int32_t nthreads, int32_t backlog);
    int32_t httpserver_bindsocket(int32_t port, int32_t backlog);
    int32_t cmdDistribution(struct evhttp_request *req);
    int32_t httpSend(struct evhttp_request *req,string info);

    //任务分发
    int32_t cmdPlatformManagement(struct evkeyvalq &params,string &reInfo);
    int32_t cmdBusinessManagement(struct evkeyvalq &params,string &reInfo);

    //第一次初始化操作
    int32_t initialize();
    void read_config();


private:
    string               httpd_option_listen ;
    int32_t		 httpd_option_port ;
    int32_t		 httpd_option_timeout ; //in seconds
    int32_t              httpd_option_thread;

public:
    TaskInterface	*m_business;
};

#endif
