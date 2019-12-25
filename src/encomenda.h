#ifndef ENCOMENDA_H
#define ENCOMENDA_H

#define ENCOMENDA_TIPO_URGENTE       ( (uint8_t)   1 )
#define ENCOMENDA_TIPO_FRAGIL        ( (uint8_t)   2 )
#define ENCOMENDA_TIPO_PESADO        ( (uint8_t)   4 )
#define ENCOMENDA_TIPO_VOLUMOSO      ( (uint8_t)   8 )
#define ENCOMENDA_ESTADO_EM_ENTREGA  ( (uint8_t)  16 )
#define ENCOMENDA_ESTADO_EXPEDIDA    ( (uint8_t)  32 )
#define ENCOMENDA_ESTADO_ENTREGUE    ( (uint8_t)  64 )
#define ENCOMENDA_ESTADO_CANCELADA   ( (uint8_t) 128 )

#include <stdint.h>
#include <stdio.h>
#include "morada.h"
#include "artigo.h"
#include "utilizador.h"

#ifndef artigovec_H
#define artigovec_H
#define VEC_TYPE        artigo
#define VEC_NAME        artigovec
#define VEC_DEALOC(X)   freeArtigo(&X)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#endif

typedef struct {
    uint64_t REGULAR ;
    uint64_t URGENTE ;
    uint64_t VOLUMOSO;
    uint64_t FRAGIL  ;
    uint64_t PESADO  ;
    uint64_t POR_KM  ;
    _Float32 MULT_CP [10][10]; // Origem Destino
} precos_tt_cent;


typedef struct {
    artigovec      artigos;
    morada         origem;
    morada         destino;
    uint64_t       distancia_km;
    uint8_t        tipoEstado;
    precos_tt_cent precos;
    uint8_t        NIF_cliente[9];
} encomenda;

int       save_precos          (FILE* f, precos_tt_cent* data);
int       save_encomenda       (FILE* f, encomenda* data);

int       encomenda_ePesado    (uint64_t a);
int       encomenda_eVolumoso  (uint64_t a);
encomenda newEncomenda         ();
void      freeEncomenda        (encomenda* e);
encomenda encomenda_formalizar (artigovec artigos, precos_tt_cent precos, uint8_t NIF[9], morada org, morada dest, uint64_t dist);
uint64_t  encomenda_CalcPreco  (encomenda* e);

void encomenda_TIPO_URGENTE      (encomenda* e);
void encomenda_TIPO_FRAGIL       (encomenda* e);
void encomenda_TIPO_FRAGIL_togle (encomenda* e);
void encomenda_TIPO_PESADO       (encomenda* e);
void encomenda_TIPO_VOLUMOSO     (encomenda* e);
void encomenda_ESTADO_EM_ENTREGA (encomenda* e);
void encomenda_ESTADO_EXPEDIDA   (encomenda* e);
void encomenda_ESTADO_ENTREGUE   (encomenda* e);
void encomenda_ESTADO_CANCELADA  (encomenda* e);

#endif