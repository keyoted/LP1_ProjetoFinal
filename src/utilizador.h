#ifndef UTILIZADOR_H
#define UTILIZADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "morada.h"

#define UTILIZADOR_CLIENTE     0
#define UTILIZADOR_DIRETOR     1
#define UTILIZADOR_DESATIVADO  2

typedef struct {
    char*   nome;           // (not null)
    uint8_t tipo;
    uint8_t NIF[9];
    uint8_t CC[12];
    morada  endereco;
} utilizador;

int        utilizador_eCCValido  (const uint8_t* const cc);
int        utilizador_eNIFValido (const uint8_t* const NIF);
utilizador newUtilizador         ();
void       freeUtilizador        (utilizador* const u);
int        save_utilizador       (FILE* const f, const utilizador* const data);
int        load_utilizador       (FILE* const f, utilizador* const data);

#endif