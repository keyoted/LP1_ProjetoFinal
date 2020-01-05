/**
 * @file    menu.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   API simples para mostrar informação e capturar input do utilizador.
 * @version 1
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 */

#ifndef MENU_H
#define MENU_H

#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "encomenda.h"
#include "utilizador.h"

#ifndef encomendavec_H
#    define encomendavec_H
#    define VEC_TYPE encomenda
#    define VEC_NAME encomendavec
#    define VEC_DEALOC(X) freeEncomenda(&X)
#    include "./vector.h"
#endif

#ifndef strvec_H
#    define strvec_H
#    define VEC_TYPE char*
#    define VEC_NAME strvec
#    define VEC_DEALOC(X) free(X)
#    include "./vector.h"
#endif

#ifndef utilizadorvec_H
#    define utilizadorvec_H
#    define VEC_TYPE utilizador
#    define VEC_NAME utilizadorvec
#    define VEC_DEALOC(X) freeUtilizador(&X)
#    include "./vector.h"
#endif

void  menu_readIntMinMax(const int min, const int max, int* const op);
int   menu_selection(const strvec* const itens);
char* menu_readString();
void  menu_readNotNulStr(char** X);
void  menu_printDiv();
void  menu_printError(const char* const err, ...);
void  menu_printInfo(const char* const info, ...);
void  menu_printHeader(const char* header);
void  menu_printUtilizador(const utilizador u);
void  menu_printArtigo(const artigo* const a);
void  menu_readInt(int* const value);
void  menu_read_Float32(_Float32* const value);
void  menu_printReciboMensal(const uint64_t ID_U, int mes, int ano, const encomendavec* const e,
                             const utilizadorvec* const uv);
void  menu_printEncomendaBrief(const encomenda* const e, const utilizadorvec* const uv);
void  menu_printEncomendaDetail(const encomenda* const e, const utilizadorvec* const uv);

#endif
