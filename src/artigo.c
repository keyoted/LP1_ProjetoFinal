#include "artigo.h"

artigo newArtigo() {
    return (artigo){
        .nome = strdup("Artigo"),
        .tratamentoEspecial = NULL,
    };
}

void freeArtigo(artigo* a) {
    if(a->nome)               freeN(a->nome);
    if(a->tratamentoEspecial) freeN(a->tratamentoEspecial);
}