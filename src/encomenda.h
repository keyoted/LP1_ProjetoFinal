#ifndef ENCOMENDA_H
#define ENCOMENDA_H

#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include "artigo.h"
#include "morada.h"
#include "precos_tt_cent.h"

#ifndef artigovec_H
#define artigovec_H
#define VEC_TYPE        artigo
#define VEC_NAME        artigovec
#define VEC_DEALOC(X)   freeArtigo(&X)
#include "./vector.h"
#endif

#define ENCOMENDA_TIPO_URGENTE       ( (uint8_t)   1 )
#define ENCOMENDA_TIPO_FRAGIL        ( (uint8_t)   2 )
#define ENCOMENDA_TIPO_PESADO        ( (uint8_t)   4 )
#define ENCOMENDA_TIPO_VOLUMOSO      ( (uint8_t)   8 )
#define ENCOMENDA_ESTADO_EM_ENTREGA  ( (uint8_t)  16 )
#define ENCOMENDA_ESTADO_EXPEDIDA    ( (uint8_t)  32 )
#define ENCOMENDA_ESTADO_ENTREGUE    ( (uint8_t)  64 )
#define ENCOMENDA_ESTADO_CANCELADA   ( (uint8_t) 128 )

typedef struct {
    artigovec      artigos;         // (size != 0)
    morada         origem;
    morada         destino;
    uint64_t       distancia_km;
    uint8_t        tipoEstado;
    precos_tt_cent precos;
    uint64_t       ID_cliente;
    time_t         criacao;
} encomenda;

int       save_encomenda       (FILE* const f, const encomenda* const data);
int       load_encomenda       (FILE* const f, encomenda* const data);
encomenda newEncomenda         ();
void      freeEncomenda        (encomenda* const e);
int       encomenda_ePesado    (const uint64_t a);
int       encomenda_eVolumoso  (const uint64_t a);
encomenda encomenda_formalizar (const artigovec artigos, const precos_tt_cent precos, const uint64_t ID_cliente, const morada org, const morada dest, const uint64_t dist);
uint64_t  encomenda_CalcPreco  (const encomenda* const e);

void encomenda_TIPO_URGENTE      (encomenda* const e);
void encomenda_TIPO_FRAGIL       (encomenda* const e);
void encomenda_TIPO_FRAGIL_togle (encomenda* const e);
void encomenda_TIPO_PESADO       (encomenda* const e);
void encomenda_TIPO_VOLUMOSO     (encomenda* const e);
void encomenda_ESTADO_EM_ENTREGA (encomenda* const e);
void encomenda_ESTADO_EXPEDIDA   (encomenda* const e);
void encomenda_ESTADO_ENTREGUE   (encomenda* const e);
void encomenda_ESTADO_CANCELADA  (encomenda* const e);

#endif