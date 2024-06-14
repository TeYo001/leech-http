#include "Utils.h"

void ssl_std_err(const SSL* ssl, int ret_val, const char* err_msg) {
    if (ret_val <= 0) {
        err(err_msg);
        #ifdef EXTRA_SSL_ERROR_INFO
        int err_code = SSL_get_error(ssl, ret_val);
        switch (err_code) {
            case SSL_ERROR_ZERO_RETURN: printf("[SSL] err: SSL_ERROR_ZERO_RETURN\n"); break;
            case SSL_ERROR_WANT_READ: printf("[SSL] err: SSL_ERROR_WANT_READ / SSL_ERROR_WANT_WRITE\n"); break;
            case SSL_ERROR_WANT_WRITE: printf("[SSL] err: SSL_ERROR_WANT_READ / SSL_ERROR_WANT_WRITE\n"); break;
            case SSL_ERROR_WANT_CONNECT: printf("[SSL] err: SSL_ERROR_WANT_CONNECT / SSL_ERROR_WANT_ACCEPT\n"); break;
            case SSL_ERROR_WANT_ACCEPT: printf("[SSL] err: SSL_ERROR_ZERO_RETURN\n"); break;
            case SSL_ERROR_WANT_X509_LOOKUP: printf("[SSL] err: SSL_ERROR_WANT_X509_LOOKUP\n"); break;
            case SSL_ERROR_WANT_ASYNC: printf("[SSL] err: SSL_ERROR_WANT_ASYNC\n"); break;
            case SSL_ERROR_WANT_ASYNC_JOB: printf("[SSL] err: SSL_ERROR_WANT_ASYNC_JOB\n"); break;
            case SSL_ERROR_WANT_CLIENT_HELLO_CB: printf("[SSL] err: SSL_ERROR_WANT_CLIENT_HELLO_CB\n"); break;
            case SSL_ERROR_SYSCALL: printf("[SSL] err: SSL_ERROR_SYSCALL\n"); break;
            case SSL_ERROR_SSL: printf("[SSL] err: SSL_ERROR_SSL\n"); break;
            default: printf("[SSL] err: Impossible to reach!\n"); break;
        }
        #endif
    }
}
