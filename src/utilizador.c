#include "utilizador.h"

int utilizador_eCCValido (uint8_t* cc) {
    int i = 0;
    // Assegurar 9 digitos
    for(; i < 9; ++i)
        if(!isdigit(cc[i]))
            return 0;
    // Assegurar 2 digitos alfanuméricos
    for(; i < 11; ++i) {
        if(isalnum(cc[i])) {
            cc[i] = (char)toupper(cc[i]);
        } else return 0;
    }
    // Asegurar ultimo digito
    if(! isdigit(cc[11])) return 0;

    return 1;
}

int utilizador_eNIFValido (uint8_t* NIF) {
    int i = 0;
    // Assegurar 9 digitos
    for(; i < 9; ++i)
        if(!isdigit(NIF[i]))
            return 0;

    return 1;
}

utilizador newUtilizador() {
    return (utilizador) {
        .tipo = UTILIZADOR_DESATIVADO,
        .endereco = newMorada(),
        .nome = strdup("Utilizador")
    };
}

void freeUtilizador(utilizador* u) {
    freeN(u->nome);
    freeMorada(&(u->endereco));
}

/*
    - utilizador utilizadores
        - str nome
        * uint8_t NIF[9]
        * uint8_t CC[12]
        - morada endereco
        * uint8_t tipo
*/
int save_utilizador (FILE* f, utilizador* data) {
    int written = 0;
    written += save_str(f, data->nome);
    written += fwrite(&(data->NIF), sizeof(uint8_t), 9, f);
    written += fwrite(&(data->CC), sizeof(uint8_t), 12, f);
    written += save_morada(f, &(data->endereco));
    written += fwrite(&(data->tipo), sizeof(uint8_t), 1, f);
    return written == (1+9+12+1+1);
}

int load_utilizador (FILE* f, utilizador* data) {
    int written = 0;
    written += load_str(f, &(data->nome));
    written += fread(&(data->NIF), sizeof(uint8_t), 9, f);
    written += fread(&(data->CC), sizeof(uint8_t), 12, f);
    written += load_morada(f, &(data->endereco));
    written += fread(&(data->tipo), sizeof(uint8_t), 1, f);
    return written == (1+9+12+1+1);
}