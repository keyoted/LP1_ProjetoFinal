#ifndef MORADA_H
#define MORADA_H

#include <stdint.h>
#include <ctype.h>
#include "utilities.h"
#include "menu.h"

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