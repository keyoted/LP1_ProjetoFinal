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

int        utilizador_eCCValido  (uint8_t* cc);
int        utilizador_eNIFValido (uint8_t* NIF);
utilizador newUtilizador         ();
void       freeUtilizador        (utilizador* u);
int        save_utilizador       (FILE* f, utilizador* data);
int        load_utilizador       (FILE* f, utilizador* data);

#endif