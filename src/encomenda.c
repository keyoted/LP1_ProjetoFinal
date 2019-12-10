#include "encomenda.h"

encomenda* newEncomenda() {
    encomenda* e     = calloc(1, sizeof(encomenda));
    e->artigos       = artigoPvec_new();
    e->origem        = newMorada();
    e->destino       = newMorada();
    e->urgencia      = REGULAR;
    e->precoCentimos = 0;
    return e;
}

void freeEncomenda(encomenda* e) {
    artigoPVec_free(&e->artigos);
    freeMorada(e->origem);
    freeMorada(e->destino);
    free(e);
}

// TODO: Implementar
uint64_t encomendaCalcPreco (encomenda* e) {
    return 100;
}