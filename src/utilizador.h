/**
 * @file    utilizador.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Capaz de gravar todas as informações sobre um utilizador.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#ifndef UTILIZADOR_H
#define UTILIZADOR_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "morada.h"

/**
 * @def UTILIZADOR_CLIENTE
 *          Define um utilizador como sendo um cliente.
 * @def UTILIZADOR_DIRETOR
 *          Define um utilizador como sendo um diretor.
 * @def UTILIZADOR_DESATIVADO
 *          Define um utilizador como estando desativado.
 */
#define UTILIZADOR_CLIENTE 0
#define UTILIZADOR_DIRETOR 1
#define UTILIZADOR_DESATIVADO 2

/**
 * Capaz de gravar todas as informações sobre um utilizador.
 */
typedef struct {
    char*   nome;     ///< Nome do cliente.
    uint8_t tipo;     ///< Tipo do cliente.
    uint8_t NIF[9];   ///< NIF do cliente.
    uint8_t CC[12];   ///< Número de cartão de cidadão do cliente.
    morada  endereco; ///< Endereço do cliente.
} utilizador;

int        utilizador_eCCValido(const uint8_t* const cc);
int        utilizador_eNIFValido(const uint8_t* const NIF);
utilizador newUtilizador();
void       freeUtilizador(utilizador* const u);
int        save_utilizador(FILE* const f, const utilizador* const data);
int        load_utilizador(FILE* const f, utilizador* const data);

#endif
