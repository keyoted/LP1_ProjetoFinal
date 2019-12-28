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
#include <time.h>
#include "morada.h"
#include "artigo.h"
#include "utilizador.h"
#include "precos_tt_cent.h"

#ifndef artigovec_H
#define artigovec_H
#define VEC_TYPE        artigo
#define VEC_NAME        artigovec
#define VEC_DEALOC(X)   freeArtigo(&X)
#include "./vector.h"
#endif

typedef struct {
    artigovec      artigos;
    morada         origem;
    morada         destino;
    uint64_t       distancia_km;
    uint8_t        tipoEstado;
    precos_tt_cent precos;
    uint8_t        NIF_cliente[9];
    time_t         criacao;
} encomenda;

int       save_encomenda       (FILE* f, encomenda* data);
int       load_encomenda       (FILE* f, encomenda* data);
encomenda newEncomenda         ();
void      freeEncomenda        (encomenda* e);
int       encomenda_ePesado    (uint64_t a);
int       encomenda_eVolumoso  (uint64_t a);
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