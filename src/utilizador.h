#define CLIENTE     0
#define DIRETOR     1
#define DESATIVADO  2

#include <stdint.h>
#include "morada.h"

typedef struct {
    char*   nome;
    uint8_t NIF[9];
    uint8_t CC[12];
    morada* adereco;
    uint8_t tipo;
} utilizador;

int         utilizador_eCCValido (uint8_t cc[12]);
utilizador* newUtilizador        ();
void        freeUtilizador       (utilizador* u);