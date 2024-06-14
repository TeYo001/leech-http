#pragma once

#include "openssl/ssl.h"

#define SERVER_SUCCESS 0
#define SERVER_FAILIURE 1

void run_server(int port);
void run_client(const char* server_address_str, int port);
void* handle_client(void* arg);
void https_server_handshake(int client_fd);
void https_client_handshake(int client_fd);
void purge_port(int port);
void pre_master_secret_extractor(const SSL* ssl, const char* pre_master_secret_str);
