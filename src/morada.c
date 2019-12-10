#include "morada.h"

morada newMorada() {
    return (morada){ .morada = strdup("undef") };
}

void freeMorada(morada m) {
    free(m.morada);
}