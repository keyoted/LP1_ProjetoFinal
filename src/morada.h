#ifndef MORADA_H
#define MORADA_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

typedef struct {
    char*   morada;             // (not null)
    uint8_t codigoPostal[7];
} morada;

morada newMorada        ();
void   freeMorada       (morada* m);
int    morada_eCPValido (uint8_t* CP);
morada morada_dup       (morada m);
int    save_morada      (FILE* f, morada* data);
int    load_morada      (FILE* f, morada* data);

#endif