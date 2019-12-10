#include "utilizador.h"

int utilizador_eCCValido (uint8_t cc[12]) {
    int i = 0;
    // Assegurar 9 digitos
    for(i; i < 9; ++i)
        if(!isdigit(cc[i]))
            return 0;
    // Assegurar 2 digitos alfanuméricos
    for(i; i < 11; ++i) {
        if(isalnum(cc[i])) {
            cc[i] = (char)toupper(cc[i]);
        } else return 0;
    }
    // Asegurar ultimo digito
    if(! isdigit(cc[11])) return 0;

    return 1;
}

utilizador* newUtilizador() {
    utilizador* u = calloc(1, sizeof(utilizador));
    u->adereco    = newMorada();
    u->tipo       = CLIENTE;
}

void freeUtilizador(utilizador* u) {
    free(u->nome);
    freeMorada(u->adereco);
    free(u);
}