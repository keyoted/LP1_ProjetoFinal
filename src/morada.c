#include "morada.h"

morada newMorada() {
    return (morada) {
        .morada = strdup("Rua")
    };
}

void freeMorada(morada* m) {
    freeN(m->morada);
}

int morada_eCPValido (uint8_t* CP) {
    for(int i = 0; i < 7; ++i)
        if(!isdigit(CP[i]))
            return 0;

    return 1;
}

morada morada_dup (morada m) {
    morada mdup = m;
    mdup.morada = strdup(m.morada);
    return mdup;
}

/*
    - morada endereco
        - str morada
        * uint8_t codigoPostal[7]
*/
int save_morada (FILE* f, morada* data) {
    int written = 0;
    written += save_str(f, data->morada);
    written += fwrite(data->codigoPostal, sizeof(uint8_t), 7, f);
    return written == 8;
}

int load_morada (FILE* f, morada* data) {
    int written = 0;
    written += load_str(f, &(data->morada));
    written += fread(data->codigoPostal, sizeof(uint8_t), 7, f);
    return written == 8;
}