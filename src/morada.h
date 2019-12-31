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
void   freeMorada       (morada* const m);
int    morada_eCPValido (const uint8_t* const CP);
morada morada_dup       (const morada m);
int    save_morada      (FILE* const f, const morada* const data);
int    load_morada      (FILE* const f, morada* const data);

#endif