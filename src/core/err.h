// Simple error handling
// (c) 2019 Jani Nykänen

#ifndef __ERR__
#define __ERR__

#include <stdbool.h>

#define ERR_MAX_LENGTH 128

// Initialize error handling
void err_init();

// Throw an error
void err_throw_no_param(const char* msg);
void err_throw_param_1(const char* msg, const char* param);

// Get an error. Returns NULL
// if does not exist
char* get_error();

#endif // __ERR__
