#include "artigo.h"

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