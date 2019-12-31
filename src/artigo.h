#ifndef ARTIGO_H
#define ARTIGO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    char*    nome;                // Nome do artigo (not null)
    char*    tratamentoEspecial;  // Texto sobre o tratamento especial do artigo
    uint64_t peso_gramas;         // Peso do artigo em gramas
    uint64_t cmCubicos;           // Volume do artigo em centimetros cubicos
} artigo;

artigo   newArtigo     ();
void     freeArtigo    (artigo* const a);
int      save_artigo   (FILE* const f, const artigo* const data);
int      load_artigo   (FILE* const f, artigo* data);

#endif