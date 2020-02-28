#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif
#else
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <event2/bufferevent.h>
#include <event2/bufferevent_ssl.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>
#include <event.h>

#include "web_ssl.hpp"
#include "header.h"

#ifdef EVENT__HAVE_NETINET_IN_H
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#endif

#ifdef _WIN32
#define stat _stat
#define fstat _fstat
#define open _open
#define close _close
#define O_RDONLY _O_RDONLY
#endif


namespace Web
{
    class Web_server
    {
    public:
        enum Scheme_type {HTTP,HTTPS};
        Web_server(Scheme_type type = HTTP,int port=8080,int nthread=2,int timeout=3);
        ~Web_server();

        bool start(http_callback callback);

    private:
        func_code do_openssl();
        func_code do_server();
        func_code socket_bind(uint16_t port,int &fd);
        static struct bufferevent* bevcb (struct event_base *base, void *arg);
        void thread_work(struct event_base *base);
        static void request_handle(struct evhttp_request *req, void *arg);
        void  err(const std::string msg);


    private:
        http_callback           callback_ {nullptr};
        SSL_CTX                 *ctx_;
        Scheme_type             type_ {HTTP};
        int                     timeout_;//seconds
        int                     port_;
        int                     nthread_;
    };
};

