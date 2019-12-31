#include "morada.h"

#include "menu.h"
#include "utilities.h"

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
    morada mcp = (morada) {
        .morada = strdup(m.morada)
    };
    memcpy(&(mcp.codigoPostal[0]), &(m.codigoPostal[0]), 9);
    return mcp;
}

/*
    - morada endereco
        - str morada
        * uint8_t codigoPostal[7]
*/
int save_morada (FILE* f, morada* data) {
    int written = 0;
    written += save_str(f, data->morada);
    if(!data->morada) {
        menu_printError("ao gravar morada - morada inválida.");
        data->morada = strdup("Inválido");
        written = 0;
    }
    written += fwrite(data->codigoPostal, sizeof(uint8_t), 7, f);
    return written == 8;
}

int load_morada (FILE* f, morada* data) {
    int written = 0;
    written += load_str(f, &(data->morada));
    if(!data->morada) {
        menu_printError("ao carregar morada - morada inválida.");
        data->morada = strdup("Inválido");
        written = 0;
    }
    written += fread(data->codigoPostal, sizeof(uint8_t), 7, f);
    return written == 8;
}