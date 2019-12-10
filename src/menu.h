#ifndef MENU_H
#define MENU_H

#ifndef  strVec_H
#define  strVec_H
#define  VEC_TYPE             char*
#define  VEC_NAME             strVec
#define  VEC_DEALOC(X)        free(X)
#include "./vector.h"
#undef   VEC_TYPE
#undef   VEC_NAME
#undef   VEC_DEALOC
#endif

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

// lê op entre [min, max]
// retorna 0 em erro, 1 de outro modo
int readIntMinMax(int min, int max, int* const op);

// retorna o numero selecionado
int menuSelection(const strVec itens) ;

int printItemPredicate(char* item, int* userdata);


#endif
