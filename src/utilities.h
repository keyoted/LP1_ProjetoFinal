/**
 * @file    utilities.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Algumas utilidades comuns.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* strdup(const char* const s);
int   vecPrintItemPredicate(char* const item, size_t* const userdata);
int   save_str(FILE* const f, const char* const data);
int   load_str(FILE* const f, char** const data);

/**
 * @def freeN(X)
 *          Liberta e anula X.
 * @def MACRO_QUOTE(X)
 *          Adiciona o simbolo (") antes e depois do indenificador X.
 * @def __MACRO_QUOTE_INTERNAL(X)
 *          Auxiliar para avaliar X.
 * @def menu_readNotNulStr(&X)
 *          Liberta X e lê uma string vâlida, guardando-a em X.
 * @def protectVarFcnCall(VAR, FCN, ERRMSG)
 *          Realiza VAR = FCN(), e se var for nullo imprime o erro ERRMSG e
 *          termina o programa.
 * @def protectFcnCall(FCN, ERRMSG)
 *          Se FCN() retornar falso, imprime o erro ERRMSG e termina o programa.
 */
#define freeN(X)                                                                                                       \
    if (X) {                                                                                                           \
        free(X);                                                                                                       \
        X = NULL;                                                                                                      \
    }
#define protectStr(X) ((X) ? (X) : ("N/A"))

#define MACRO_QUOTE(X) __MACRO_QUOTE_INTERNAL(X)
#define __MACRO_QUOTE_INTERNAL(X) #X

#ifdef DEBUG_BUILD
#    define protectVarFcnCall(VAR, FCN, ERRMSG)                                                                        \
        VAR = FCN;                                                                                                     \
        if (!VAR) {                                                                                                    \
            printf("File: [" __FILE__ "] Funcion: [%s] Line: [" MACRO_QUOTE(__LINE__) "]\n", __func__);                \
            printf(ERRMSG);                                                                                            \
            exit(EXIT_FAILURE);                                                                                        \
        }

#    define protectFcnCall(FCN, ERRMSG)                                                                                \
        if (!FCN) {                                                                                                    \
            printf("File: " __FILE__ "        Funcion: %s        Line: " MACRO_QUOTE(__LINE__) "\n", __func__);        \
            printf(ERRMSG);                                                                                            \
            exit(EXIT_FAILURE);                                                                                        \
        }
#else
#    define protectVarFcnCall(VAR, FCN, ERRMSG)                                                                        \
        VAR = FCN;                                                                                                     \
        if (!VAR) {                                                                                                    \
            printf(ERRMSG);                                                                                            \
            exit(EXIT_FAILURE);                                                                                        \
        }

#    define protectFcnCall(FCN, ERRMSG)                                                                                \
        if (!FCN) {                                                                                                    \
            printf(ERRMSG);                                                                                            \
            exit(EXIT_FAILURE);                                                                                        \
        }
#endif


#endif