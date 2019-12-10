#include "encomenda.h"

encomenda newEncomenda() {
    return (encomenda) {
        .artigos       = artigovec_new(),
        .origem        = newMorada(),
        .destino       = newMorada(),
        .urgencia      = REGULAR,
        .precoCentimos = 0,
        .estado        = EM_ENTREGA
    };
}

void freeEncomenda(encomenda e) {
    artigovec_free(&e.artigos);
    freeMorada(e.origem);
    freeMorada(e.destino);
}

// TODO: Implementar
uint64_t encomendaCalcPreco (encomenda e) {
    return 100;
}