#ifndef ARTIGO_H
#define ARTIGO_H

#include <stdint.h>
#include "utilities.h"

typedef struct {
    char*    nome;                // Nome do artigo
    char*    tratamentoEspecial;  // Texto sobre o tratamento especial do artigo
    uint64_t peso_gramas;         // Peso do artigo em gramas
    uint64_t cmCubicos;           // Volume do artigo em centimetros cubicos
} artigo;

artigo   newArtigo     ();
void     freeArtigo    (artigo* a);
int      save_artigo   (FILE* f, artigo* data);

#endif