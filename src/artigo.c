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

/*
    - artigo artigos
        - str    nome
        - str    tratamentoEspecial
        * uint64_t peso_gramas
        * uint64_t cmCubicos
*/
int save_artigo (FILE* f, artigo* data) {
    int written = 0;
    written += save_str(f, data->nome);
    written += save_str(f, data->tratamentoEspecial);
    written += fwrite(&(data->peso_gramas), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->cmCubicos), sizeof(uint64_t), 1, f);
    return written == 4;
}

int load_artigo (FILE* f, artigo* data) {
    int written = 0;
    written += load_str(f, &(data->nome));
    written += load_str(f, &(data->tratamentoEspecial));
    written += fread(&(data->peso_gramas), sizeof(uint64_t), 1, f);
    written += fread(&(data->cmCubicos), sizeof(uint64_t), 1, f);
    return written == 4;
}

