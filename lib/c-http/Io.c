#include "Io.h"
#include "Utils.h"
#include "stdlib.h"

void write_to_file(const char* file_name, const char* msg) {
    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        err("failed to open file");
        exit(EXIT_FAILURE);
    }
    if (fprintf(file, "%s", msg) < 0) {
        err("failed to write to file");
        exit(EXIT_FAILURE);
    }
}

void append_to_file(const char* file_name, const char* msg) {
    FILE* file = fopen(file_name, "wb");
    if (file == NULL) {
        err("failed to open file");
        exit(EXIT_FAILURE);
    }
    if (fprintf(file, "%s", msg) < 0) {
        err("failed to write to file");
        exit(EXIT_FAILURE);
    }}

void read_file(const char* file_name, unsigned int max_buffer_length, char* out_buffer) {
    FILE* file = fopen(file_name, "r");
    char ch;
    unsigned int current_length = 0;

    if (file == NULL) {
        err("failed to open file");
        exit(EXIT_FAILURE);
    }
    while ((ch = fgetc(file)) != EOF) {
        if (current_length == max_buffer_length) {
            err("exceeded the maximum buffer length");
            exit(EXIT_FAILURE);
        }
        out_buffer[current_length] = ch;
        current_length++;
    }
}

void parse_file_char(const char* file_name, file_char_parser parser) {
    err("not implemented yet");
    exit(EXIT_FAILURE);
}
