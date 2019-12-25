#ifndef UTILIZADOR_H
#define UTILIZADOR_H

#define UTILIZADOR_CLIENTE     0
#define UTILIZADOR_DIRETOR     1
#define UTILIZADOR_DESATIVADO  2

#include <stdint.h>
#include <stdlib.h>
#include "morada.h"
#include "utilities.h"

typedef struct {
    char*   nome;
    uint8_t NIF[9];
    uint8_t CC[12];
    morada  endereco;
    uint8_t tipo;
} utilizador;

int        utilizador_eCCValido  (uint8_t* cc);
int        utilizador_eNIFValido (uint8_t* NIF);
utilizador newUtilizador         ();
void       freeUtilizador        (utilizador* u);

#endif