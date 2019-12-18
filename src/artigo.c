#include "artigo.h"

int artigo_ePesado(artigo a) {
    return a.peso_gramas > 20000;
}

int artigo_eVolumoso(artigo a) {
    return a.milimetrosCubicos > (500 * 500 * 500);
}

artigo newArtigo() {
    return (artigo){
        .nome = strdup("undef"),
        .tratamentoEspecial = strdup("\0"),
        .tipoDeTransporte = ARTIGO_REGULAR
    };
}

void freeArtigo(artigo a) {
    free(a.nome);
    free(a.tratamentoEspecial);
}

uint64_t artigo_precoTipoTransporte (artigo a, uint64_t tabelaPrecos[5]) {
    if(artigo_ePesado(a))               a.tipoDeTransporte = (a.tipoDeTransporte || ARTIGO_PESADO)   && (~ARTIGO_REGULAR);
    if(artigo_eVolumoso(a))             a.tipoDeTransporte = (a.tipoDeTransporte || ARTIGO_VOLUMOSO) && (~ARTIGO_REGULAR);
    if(a.tratamentoEspecial[0] != '\0') a.tipoDeTransporte = (a.tipoDeTransporte || ARTIGO_FRAGIL)   && (~ARTIGO_REGULAR);

    if(a.tipoDeTransporte && ARTIGO_REGULAR) return tabelaPrecos[TABELA_ARTIGO_REGULAR];

    uint64_t soma = 0;
    if(a.tipoDeTransporte && ARTIGO_URGENTE ) soma += tabelaPrecos[TABELA_ARTIGO_URGENTE ]; 
    if(a.tipoDeTransporte && ARTIGO_FRAGIL  ) soma += tabelaPrecos[TABELA_ARTIGO_FRAGIL  ]; 
    if(a.tipoDeTransporte && ARTIGO_PESADO  ) soma += tabelaPrecos[TABELA_ARTIGO_PESADO  ]; 
    if(a.tipoDeTransporte && ARTIGO_VOLUMOSO) soma += tabelaPrecos[TABELA_ARTIGO_VOLUMOSO]; 
    return soma;
}