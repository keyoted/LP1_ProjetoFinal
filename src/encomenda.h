#ifndef ENCOMENDA_H
#define ENCOMENDA_H

enum ARTIGOS_TIPOS {
     ENCOMENDA_TIPO_URGENTE  = 0x1,
     ENCOMENDA_TIPO_FRAGIL   = 0x2,
     ENCOMENDA_TIPO_PESADO   = 0x4,
     ENCOMENDA_TIPO_VOLUMOSO = 0x8,
};

enum precoTipoDeTransporte {           // Enum para auxiliar com a indexação de tabelaPrecoTipoTransporte_cent
     ENCOMENDA_REGULAR    =  0,
     ENCOMENDA_URGENTE    =  1,
     ENCOMENDA_VOLUMOSO   =  2,
     ENCOMENDA_FRAGIL     =  3,
     ENCOMENDA_PESADO     =  4,
     ENCOMENDA_POR_KM     =  5,

     ENCOMENDA_EXPEDIDA   =  6,
     ENCOMENDA_CANCELADA  =  7,
     ENCOMENDA_EM_ENTREGA =  8,
     ENCOMENDA_ENTREGUE   =  9,
};

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
    artigovec  artigos;
    morada     origem;
    morada     destino;

    uint8_t    tipoTransporte;
    float      multiplicador_CP;
    uint64_t   precoFinalTipoTransporte_cent;
    uint64_t   precoPorKm_cent;
    uint64_t   precoFinal_cent;

    uint8_t    NIF_cliente[9];
    uint8_t    estado;
} encomenda;

int       encomenda_ePesado   (encomenda a);
int       encomenda_eVolumoso (encomenda a);
encomenda newEncomenda        ();
void      freeEncomenda       (encomenda e);
uint64_t  encomenda_CalcPreco (encomenda e, uint64_t precoKM, float multiplicadorPreco[10][10]);

#endif