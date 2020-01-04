/**
 * @file    artigo.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Um artigo é um objeto com um nome, peso e volume que pode ou não
 *          requerer tratamento especial.
 * @version 1
 * @date    2019-12-31
 * @copyright Copyright (c) 2019
 */

#ifndef ARTIGO_H
#define ARTIGO_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Um artigo é um objeto com um nome, peso e volume que pode ou não requerer
 * tratamento especial.
 */
typedef struct {
    char* nome;               ///< Nome do artigo, não pode ser NULL.
    char* tratamentoEspecial; ///< Texto sobre o tratamento especial do artigo,
                              ///< ou NULL.
    uint64_t peso_gramas;     ///< Peso do artigo em gramas.
    uint64_t cmCubicos;       ///< Volume do artigo em centimetros cubicos.
} artigo;

artigo newArtigo();
void   freeArtigo(artigo* const a);
int    save_artigo(FILE* const f, const artigo* const data);
int    load_artigo(FILE* const f, artigo* data);

#endif