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
        .tratamentoEspecial = NULL,
    };
}

void freeArtigo(artigo a) {
    if(a.nome)               free(a.nome);
    if(a.tratamentoEspecial) free(a.tratamentoEspecial);
}