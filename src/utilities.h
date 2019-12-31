#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char*   strdup                (const char *s);
int     vecPrintItemPredicate (char* item, int* userdata);
int     save_str              (FILE* f, char* data);
int     load_str              (FILE* f, char** data);
#define freeN(X)              { if(X) { free(X); X = NULL; } }
#define protectStr(X)         ((X)?(X):("N/A"))

#define readNotNulStr(X)            \
    if(X) {                         \
        free(X);                    \
        X = NULL;                   \
    }                               \
    while (!X) {                    \
        printf("$ ");               \
        X = menu_readString();      \
    }

#define protectVarFcnCall(VAR, FCN, ERRMSG)     \
    {                                           \
        VAR = FCN;                              \
        if(!VAR) {                              \
            printf(ERRMSG);                     \
            exit(EXIT_FAILURE);                 \
        }                                       \
    }

#define protectFcnCall(FCN, ERRMSG)     \
    {                                   \
        if(!FCN) {                      \
            printf(ERRMSG);             \
            exit(EXIT_FAILURE);         \
        }                               \
    }

#endif