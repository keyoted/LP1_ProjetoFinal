/**
 * @file    encomenda.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Uma encomenda é um conjunto de artigos transportados de um certo
 *          adereço de origem para um adereço de destino.
 *          A encomenda percorre uma certa distância e tem diversos estados
 *          como ENCOMENDA_ESTADO_EM_ENTREGA ou tipos como
 *          ENCOMENDA_TIPO_URGENTE.
 *          Adicionalmente a encomenda armazena a tabela de preços ativa quando
 *          esta foi criada, a data de criação e o ID do cliente que criou a
 *          encomenda.
 * @version 1
 * @date 2019-12-31
 *
 * @copyright Copyright (c) 2019
 */

/**
 * @def ENCOMENDA_TIPO_URGENTE
 *          Define uma encomenda que tem que ser entregada urgentemente.
 * @def ENCOMENDA_TIPO_FRAGIL
 *          Define uma encomenda que é frágil.
 * @def ENCOMENDA_TIPO_PESADO
 *          Define uma encomenda que tem que é pesada.
 * @def ENCOMENDA_TIPO_VOLUMOSO
 *          Define uma encomenda que é volumosa.
 * @def ENCOMENDA_ESTADO_EM_ENTREGA
 *          Define uma encomenda que esta a ser processada mas ainda não foi
 *          expedida.
 * @def ENCOMENDA_ESTADO_EXPEDIDA
 *          Define uma encomenda que está a ser transportada até ao seu destino.
 * @def ENCOMENDA_ESTADO_ENTREGUE
 *          Define uma encomenda que foi entregue ao seu destino.
 * @def ENCOMENDA_ESTADO_CANCELADA
 *          Define uma encomenda que foi cancelada e não será expedida.
 */

#ifndef ENCOMENDA_H
#define ENCOMENDA_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "artigo.h"
#include "morada.h"
#include "precos_cent.h"

#ifndef artigovec_H
#    define artigovec_H
#    define VEC_TYPE artigo
#    define VEC_NAME artigovec
#    define VEC_DEALOC(X) freeArtigo(&X)
#    define VEC_WRITE(X, F) save_artigo(F, X)
#    define VEC_READ(X, F) load_artigo(F, X)
#    include "./vector.h"
#endif

#define ENCOMENDA_TIPO_URGENTE ((uint8_t) 1)
#define ENCOMENDA_TIPO_FRAGIL ((uint8_t) 2)
#define ENCOMENDA_TIPO_PESADO ((uint8_t) 4)
#define ENCOMENDA_TIPO_VOLUMOSO ((uint8_t) 8)
#define ENCOMENDA_ESTADO_EM_ENTREGA ((uint8_t) 16)
#define ENCOMENDA_ESTADO_EXPEDIDA ((uint8_t) 32)
#define ENCOMENDA_ESTADO_ENTREGUE ((uint8_t) 64)
#define ENCOMENDA_ESTADO_CANCELADA ((uint8_t) 128)

/**
 * A encomenda percorre uma certa distância e tem diversos estados como
 * ENCOMENDA_ESTADO_EM_ENTREGA ou tipos como ENCOMENDA_TIPO_URGENTE.
 * Adicionalmente a encomenda armazena a tabela de preços ativa quando esta foi
 * criada, a data de criação e o ID do cliente que criou a encomenda.
 */
typedef struct {
    artigovec artigos;  ///< Artigos que fazem parte da encomenda.
    morada    origem;   ///< Origem da encomenda.
    morada    destino;  ///< Destino da encomenda.
    uint8_t tipoEstado; ///< Os quatro bits de baixo são flags e indicam o tipo da encomenda, os quatro de cima são o
                        ///< estado e apenas um dos bits deverá de estar ativo.
    uint64_t    distancia_km; ///< Distancia(em km) da origem até ao destino.
    precos_cent precos;       ///< Tabela de preços na altura em que a encomenda foi formalizada.
    time_t      criacao;      ///< Data da criação da encomenda.
    uint64_t    ID_cliente;   ///< ID do cliente que formalizou a encomenda.
} encomenda;

encomenda newEncomenda();
void      freeEncomenda(encomenda* const e);
encomenda encomenda_formalizar(const artigovec artigos, const precos_cent precos, const uint64_t ID_cliente,
                               const morada org, const morada dest, const uint64_t dist);

int save_encomenda(FILE* const f, const encomenda* const data);
int load_encomenda(FILE* const f, encomenda* const data);

int      encomenda_ePesado(const uint64_t a);
int      encomenda_eVolumoso(const uint64_t a);
uint64_t encomenda_CalcPreco(const encomenda* const e);

void encomenda_TIPO_URGENTE(encomenda* const e);
void encomenda_TIPO_FRAGIL(encomenda* const e);
void encomenda_TIPO_FRAGIL_togle(encomenda* const e);
void encomenda_TIPO_PESADO(encomenda* const e);
void encomenda_TIPO_VOLUMOSO(encomenda* const e);
void encomenda_ESTADO_EM_ENTREGA(encomenda* const e);
void encomenda_ESTADO_EXPEDIDA(encomenda* const e);
void encomenda_ESTADO_ENTREGUE(encomenda* const e);
void encomenda_ESTADO_CANCELADA(encomenda* const e);

#endif