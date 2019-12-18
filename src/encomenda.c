#include "encomenda.h"

encomenda newEncomenda() {
    return (encomenda) {
        .artigos        = artigovec_new(),
        .origem         = newMorada(),
        .destino        = newMorada(),
        .tipoTransporte = 0,
        .estado         = ENCOMENDA_EM_ENTREGA
    };
}

void freeEncomenda(encomenda e) {
    artigovec_free(&e.artigos);
    freeMorada(e.origem);
    freeMorada(e.destino);
}

// TODO: Corrigir
uint64_t encomenda_CalcPreco (encomenda e, uint64_t precoKM, float multiplicadorPreco[10][10]) {
    /*
    // ENCOMENDA_TIPO_PESADO  
    if(encomenda_ePesado(e))   e.tipoTransporte = (e.tipoTransporte || ENCOMENDA_TIPO_PESADO);
    // ENCOMENDA_TIPO_VOLUMOSO
    if(encomenda_eVolumoso(e)) e.tipoTransporte = (e.tipoTransporte || ENCOMENDA_TIPO_VOLUMOSO);
    // ENCOMENDA_TIPO_FRAGIL  
    for(size_t i = 0; i < e.artigos.size; ++i) {
        if(e.artigos.data[i].tratamentoEspecial != NULL) {
            e.tipoTransporte = (e.tipoTransporte || ENCOMENDA_TIPO_FRAGIL);
            break;
        }
    }

    if(a.tipoDeTransporte && ARTIGO_REGULAR) return tabelaPrecos[TABELA_ARTIGO_REGULAR];

    uint64_t soma = 0;
    if(a.tipoDeTransporte && ARTIGO_URGENTE ) soma += tabelaPrecos[TABELA_ARTIGO_URGENTE ]; 
    if(a.tipoDeTransporte && ARTIGO_FRAGIL  ) soma += tabelaPrecos[TABELA_ARTIGO_FRAGIL  ]; 
    if(a.tipoDeTransporte && ARTIGO_PESADO  ) soma += tabelaPrecos[TABELA_ARTIGO_PESADO  ]; 
    if(a.tipoDeTransporte && ARTIGO_VOLUMOSO) soma += tabelaPrecos[TABELA_ARTIGO_VOLUMOSO]; 
    return soma;

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

    return soma;*/
    return 10;
}