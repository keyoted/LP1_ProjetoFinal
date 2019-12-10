#define URGENTE 1
#define REGULAR 0

#ifndef artigoPVec_H
#define artigoPVec_H
#define VEC_TYPE        artigo*
#define VEC_NAME        artigoPvec
#define VEC_DEALOC(X)   freeArtigo(X)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#endif

#include "stdint.h"
#include "morada.h"

typedef struct {
    artigoPvec  artigos;
    morada*     origem;
    morada*     destino;
    uint8_t     urgencia;
    uint64_t    precoCentimos;
    uint8_t     NIF_cliente[9];
} encomenda;

encomenda* newEncomenda       ();
void       freeEncomenda      (encomenda* e);
uint64_t   encomendaCalcPreco (encomenda* e);