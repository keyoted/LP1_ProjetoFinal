#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

char* strdup                (const char *s);
int   vecPrintItemPredicate (char* item, int* userdata);
int   save_str              (FILE* f, uint8_t* data);
#ifndef freeN
#define freeN(x) { free(x); (x) = NULL; }
#endif

#endif