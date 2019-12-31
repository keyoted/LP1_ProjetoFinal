#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char*   strdup                (const char* const s);
int     vecPrintItemPredicate (char* const item, int* const userdata);
int     save_str              (FILE* const f, const char* const data);
int     load_str              (FILE* const f, char** const data);
#define freeN(X)              { if(X) { free(X); X = NULL; } }
#define protectStr(X)         ((X)?(X):("N/A"))

#define MACRO_QUOTE(a) __MACRO_QUOTE_INTERNAL(a)
#define __MACRO_QUOTE_INTERNAL(a) #a

#define readNotNulStr(X)            \
    if(X) {                         \
        free(X);                    \
        X = NULL;                   \
    }                               \
    while (!X) {                    \
        printf("$ ");               \
        X = menu_readString();      \
    }

#ifdef DEBUG_BUILD
    #define protectVarFcnCall(VAR, FCN, ERRMSG)     \
        {                                           \
            VAR = FCN;                              \
            if(!VAR) {                              \
                printf("File: " __FILE__ "        Funcion: %s        Line: " MACRO_QUOTE(__LINE__) "\n", __func__);\
                printf(ERRMSG);                     \
                exit(EXIT_FAILURE);                 \
            }                                       \
        }

    #define protectFcnCall(FCN, ERRMSG)     \
        {                                   \
            if(!FCN) {                      \
                printf("File: " __FILE__ "        Funcion: %s        Line: " MACRO_QUOTE(__LINE__) "\n", __func__);\
                printf(ERRMSG);             \
                exit(EXIT_FAILURE);         \
            }                               \
        }
#else
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


#endif