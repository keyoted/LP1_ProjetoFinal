#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "utilities.h"
#include "encomenda.h"
#include "utilizador.h"
#include "encomenda.h"

#ifndef encomendavec_H
#define encomendavec_H
#define VEC_TYPE         encomenda
#define VEC_NAME         encomendavec
#define VEC_DEALOC(X)    freeEncomenda(&X)
#include "./vector.h"
#endif

#ifndef  strvec_H
#define  strvec_H
#define  VEC_TYPE             char*
#define  VEC_NAME             strvec
#define  VEC_DEALOC(X)        free(X)
#include "./vector.h"
#endif

#ifndef utilizadorvec_H
#define utilizadorvec_H
#define VEC_TYPE         utilizador
#define VEC_NAME         utilizadorvec
#define VEC_DEALOC(X)    freeUtilizador(&X)
#include "./vector.h"
#endif

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
void  menu_printEncomendaBrief  (encomenda* e, utilizadorvec* uv);
void  menu_printUtilizador      (utilizador u);
void  menu_printArtigo          (artigo* a);
int   menu_readInt              (int* const value);
int   menu_read_Float32         (_Float32* const value);
void  menu_printEncomendaDetail (encomenda* e, utilizadorvec* uv);
void  menu_printReciboMensal    (uint64_t ID_U, int mes, int ano, encomendavec* e, utilizadorvec* uv);

#endif
