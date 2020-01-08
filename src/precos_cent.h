/**
 * @file    precos_cent.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Capaz de capturar os preços atuais que a empresa transportadora
 * fixou. Os preços são fixados em cêntimos.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#ifndef precos_cent_h
#define precos_cent_h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Capaz de capturar os preços atuais que a empresa transportadora
 * fixou. Os preços são fixados em cêntimos.
 */
typedef struct {
    uint64_t REGULAR;         ///< Preço base de um artigo regular.
    uint64_t URGENTE;         ///< Preço base de um artigo urgente.
    uint64_t VOLUMOSO;        ///< Preço base de um artigo volumoso.
    uint64_t FRAGIL;          ///< Preço base de um artigo frágil.
    uint64_t PESADO;          ///< Preço base de um artigo pesado.
    uint64_t POR_KM;          ///< Preço por kilometro transportado.
    _Float32 MULT_CP[9][9];   ///< Mapeia o multiplicador do código postal do modo: [origem][destino].
} precos_cent;

int save_precos(FILE* const f, const precos_cent* const data);
int load_precos(FILE* const f, precos_cent* const data);

#endif