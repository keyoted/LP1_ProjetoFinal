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
    if(u->nome) freeN(u->nome);
    freeMorada(&(u->endereco));
}