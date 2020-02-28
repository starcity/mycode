#pragma once
#include <openssl/ssl.h>
#include <openssl/err.h>                                                                                        
#include <openssl/rand.h>
#include "header.h"
#include <atomic>

namespace Web{
class Web_ssl{
public:
    static Web_ssl &get_instance()
    {
        static Web_ssl web_ssl_;
        return web_ssl_;
    }

    func_code web_ssl_init()
    {
        if(!flag_.load()){
        //windows net init
    #ifdef _WIN32
        {   
            WORD wVersionRequested;
            WSADATA wsaData;
            int err;

            wVersionRequested = MAKEWORD(2, 2); 

            err = WSAStartup(wVersionRequested, &wsaData);
            if (err != 0) {
                std::string msg("WSAStartup failed with error:");
                msg += std::to_string(err);
                err(msg);
                return func_code_window_startup_wrong;
            }   
        }   
    #endif // _WIN32

        //ssl init
    #if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
        (defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
        // Initialize OpenSSL
        SSL_library_init();
        ERR_load_crypto_strings();
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();
    #endif
        flag_.store(true);
        }

        return func_code_OK;
    }

    void web_ssl_uninit()
    {
        if(flag_.load()){
        #if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
            (defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
            EVP_cleanup();
            ERR_free_strings();

        #if OPENSSL_VERSION_NUMBER < 0x10000000L
            ERR_remove_state(0);
        #else
            ERR_remove_thread_state(NULL);
        #endif

            CRYPTO_cleanup_all_ex_data();

            sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
        #endif /* (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
              (defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L) */

        #ifdef _WIN32
            WSACleanup();
        #endif
            flag_.store(false);
        }
    }
private:
    std::atomic<bool> flag_ {false};
   };
}
