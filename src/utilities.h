#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* strdup                (const char *s);
int   vecPrintItemPredicate (char* item, int* userdata);
#ifndef freeN
#define freeN(x) { free(x); (x) = NULL; }
#endif

#endif