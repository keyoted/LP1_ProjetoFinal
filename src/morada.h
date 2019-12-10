#include <stdint.h>

typedef struct {
    char*   morada;
    uint8_t codigoPostal[7];
} morada;

morada* newMorada  ();
void    freeMorada (morada* m);