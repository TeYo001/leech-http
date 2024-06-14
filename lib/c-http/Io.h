#pragma once

#include "stdio.h"

typedef void (*file_char_parser)(const char ch);

void write_to_file(const char* file_name, const char* msg);
void append_to_file(const char* file_name, const char* msg);
void read_file(const char* file_name, unsigned int max_buffer_length, char* out_buffer);
void parse_file_char(const char* file_name, file_char_parser parser);
