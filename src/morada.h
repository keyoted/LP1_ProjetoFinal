#ifndef MORADA_H
#define MORADA_H

#include <stdint.h>
#include "utilities.h"

typedef struct {
    char*   morada;
    uint8_t codigoPostal[7];
} morada;

morada newMorada  ();
void   freeMorada (morada m);

#endif