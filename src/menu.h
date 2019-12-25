#ifndef MENU_H
#define MENU_H

#ifndef  strvec_H
#define  strvec_H
#define  VEC_TYPE             char*
#define  VEC_NAME             strvec
#define  VEC_DEALOC(X)        free(X)
#include "./vector.h"
#undef   VEC_TYPE
#undef   VEC_NAME
#undef   VEC_DEALOC
#endif

#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include "utilities.h"
#include "encomenda.h"
#include "utilizador.h"

// lê op entre [min, max]
// retorna 0 em erro, 1 de outro modo
int   menu_readIntMinMax        (int min, int max, int* const op);
// retorna o numero selecionado
int   menu_selection            (const strvec* itens) ;
char* menu_readString           (FILE* fp);
void  menu_printDiv             ();
void  menu_printError           (char* err, ...);
void  menu_printInfo            (char* info, ...);
void  menu_printHeader          (char* header);
void  menu_printEncomendaBrief  (encomenda u);
void  menu_printUtilizador      (utilizador u);
void  menu_printArtigo          (artigo a);
int   menu_readInt              (int* const value);
int   menu_readFloat            (float* const value);
void  menu_printEncomendaDetail (encomenda e);

#endif
