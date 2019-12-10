#ifndef ENCOMENDA_H
#define ENCOMENDA_H

#define URGENTE 1
#define REGULAR 0

#define EXPEDIDA    0
#define CANCELADA   1
#define EM_ENTREGA  2

#include "stdint.h"
#include "morada.h"
#include "artigo.h"

#ifndef artigovec_H
#define artigovec_H
#define VEC_TYPE        artigo
#define VEC_NAME        artigovec
#define VEC_DEALOC(X)   freeArtigo(X)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#endif

typedef struct {
    artigovec artigos;
    morada     origem;
    morada     destino;
    uint8_t    urgencia;
    uint64_t   precoCentimos;
    uint8_t    NIF_cliente[9];
    uint8_t    estado;
} encomenda;

encomenda newEncomenda       ();
void      freeEncomenda      (encomenda e);
uint64_t  encomendaCalcPreco (encomenda e);

#endif