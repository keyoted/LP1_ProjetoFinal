#ifndef UTILIZADOR_H
#define UTILIZADOR_H

#define CLIENTE     0
#define DIRETOR     1
#define DESATIVADO  2

#include <stdint.h>
#include <stdlib.h>
#include "morada.h"
#include "utilities.h"

typedef struct {
    char*   nome;
    uint8_t NIF[9];
    uint8_t CC[12];
    morada  adereco;
    uint8_t tipo;
} utilizador;

int        utilizador_eCCValido  (uint8_t* cc);
int        utilizador_eNIFValido (uint8_t* NIF);
utilizador newUtilizador         ();
void       freeUtilizador        (utilizador u);

#endif