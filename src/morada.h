/**
 * @file    morada.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Estrutura que é capaz de gravar a localização de um utilizador.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#ifndef MORADA_H
#define MORADA_H

#include <ctype.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char*   morada;          ///< Morada textual(rua, nº da porta, etc)não NULL.
    uint8_t codigoPostal[7]; ///< Código postal, (números em ASCII).
} morada;

morada newMorada();
void   freeMorada(morada* const m);
int    morada_eCPValido(const uint8_t* const CP);
morada morada_dup(const morada m);
int    save_morada(FILE* const f, const morada* const data);
int    load_morada(FILE* const f, morada* const data);

#endif