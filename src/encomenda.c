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

uint64_t encomenda_CalcPreco (encomenda e, uint64_t precoKM, float multiplicadorPreco[10][10]) {
    // Σ(tipo_transporte)
    uint64_t soma = 0;
    for(uint64_t i = 0; i < e.artigos.size; ++i) {
        soma += e.artigos.data[i].precoTipoDeTransporte_cent;
    }

    // Σ(tipo_transporte)+ km*preço_km)
    // TODO: substituir 10 pela distancia em km
    soma += 10 * precoKM;
    
    // (Σ(tipo_transporte)+ km*preço_km)*fator_cp
    soma *= multiplicadorPreco[e.origem.codigoPostal[0]][e.destino.codigoPostal[0]];

    return soma;
}