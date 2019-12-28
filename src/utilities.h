#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

char*   strdup                (const char *s);
int     vecPrintItemPredicate (char* item, int* userdata);
int     save_str              (FILE* f, char* data);
int     load_str              (FILE* f, char** data);
#define freeCHK(X, Y)         { if(X) free(X); X = Y; }
#define freeN(X)              { if(X) free(X); X = NULL; }

#endif