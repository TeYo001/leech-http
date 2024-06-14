#include "Http.h"
#include "Utils.h"
#include "Io.h"

#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "pthread.h"
#include "sys/socket.h"
#include "openssl/ssl.h"

#define MAX_PENDING_SERVER_CONNECTIONS 10
#define PRE_MASTER_SECRET_FILE_NAME "PreMasterSecret.md"

void run_server(int port) {
    int server_fd;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("socket initialization failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        err("socket binding to port failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_PENDING_SERVER_CONNECTIONS) < 0) {
        err("server listening failed");
        exit(EXIT_FAILURE);
    }
    log("server started listening for connections successfully");

    while(true) {
        //struct sockaddr_in client_addr;
        //socklen_t client_addr_len = sizeof(client_addr);
        int* client_fd_ptr; // this is a ptr to detach the client_fd from the current threads memory

        if ((*client_fd_ptr = accept(server_fd, NULL, NULL)) < 0) {
            warn("failed to accept client connection");
            continue;
        }
        
        log("server connected successfully!");

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void*)client_fd_ptr);
        void* thread_return;
        pthread_join(thread_id, &thread_return);
        return;
    }
}

void run_client(const char* server_address_str, int port) {
    int client_fd;
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("socket initialization failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_address_str);
    server_addr.sin_port = htons(port);

    if (connect(client_fd, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        err("server connection failed");
        exit(EXIT_FAILURE);
    }

    /*
        char buffer[255] = "Message from the client!\n";
    send(client_fd, buffer, sizeof(buffer), 0);
    log("client sent message successfully");
    */

    https_client_handshake(client_fd);
}

void* handle_client(void* arg) {
    int client_fd = *((int*)arg);
    
    https_server_handshake(client_fd);
    
    /*
        char buffer[255];
    int message_length = recv(client_fd, buffer, sizeof(buffer), 0);
    
    flockfile(stdout);
    printf("message length: %d\n", message_length);
    printf("%s\n", buffer);
    funlockfile(stdout);
    */

    close(client_fd);
    return NULL;
}

void https_server_handshake(int client_fd) {
    flockfile(stdout);

    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    SSL* ssl = SSL_new(ctx);
    ssl_std_err(ssl, SSL_set_fd(ssl, client_fd), "server failed to set file descriptor");
    ssl_std_err(ssl, SSL_use_certificate_file(ssl, "myCA.pem", SSL_FILETYPE_PEM), "server failed to use certificate file");
    ssl_std_err(ssl, SSL_use_PrivateKey_file(ssl, "myCA.key", SSL_FILETYPE_PEM), "server failed to use private key file");
    ssl_std_err(ssl, SSL_accept(ssl), "server couldn't accept connection");

    char request_buffer[1024] = {0};
    ssl_std_err(ssl, SSL_read(ssl, request_buffer, strlen("GET")), "server failed to read request");;
    char response[1024] = "Very cool lad\r\n";
    ssl_std_err(ssl, SSL_write(ssl, response, strlen(response)), "server failed to write response");

    log("finished the ssl / https handshake");
    
    funlockfile(stdout);
    SSL_shutdown(ssl);
}

void https_client_handshake(int client_fd) {
    SSL_CTX* ctx = SSL_CTX_new(TLS_method());
    log("callback hooked");
    SSL_CTX_set_keylog_callback(ctx, pre_master_secret_extractor);
    write_to_file(PRE_MASTER_SECRET_FILE_NAME, ""); // clear file
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);
    SSL_connect(ssl);

    char* request = "GET";
    SSL_write(ssl, request, strlen(request));
    char buffer[1024] = {0};
    SSL_read(ssl, buffer, 1023);
    printf("Response:\n%s\n", buffer);

    SSL_shutdown(ssl);
}

void purge_port(int port) {
    int sock_fd;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("socket initialization failed");
        exit(EXIT_FAILURE);
    }
    close(sock_fd);
}

void pre_master_secret_extractor(const SSL* ssl, const char* pre_master_secret_str) {
    flockfile(stdout);
    log("pre master secret extractor called");
    funlockfile(stdout);
    append_to_file(PRE_MASTER_SECRET_FILE_NAME, pre_master_secret_str);
}
