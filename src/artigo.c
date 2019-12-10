#include "artigo.h"

int ePesado(artigo* a) {
    return a->gramas > 20000;
}

int eVolumoso(artigo* a) {
    return a->milimetrosCubicos > (500 * 500 * 500);
}

artigo* newArtigo() {
    return calloc(1, sizeof(artigo));
}

void freeArtigo(artigo* a) {
    free(a->nome);
    free(a->tratamentoEspecial);
    free(a);
}