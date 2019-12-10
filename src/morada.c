#include "morada.h"

morada* newMorada() {
    return calloc(1, sizeof(morada));
}

void freeMorada(morada* m) {
    free(m->morada);
    free(m);
}