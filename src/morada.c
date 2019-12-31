#include "morada.h"

#include "menu.h"
#include "utilities.h"

morada newMorada() {
    return (morada) {
        .morada = strdup("Rua")
    };
}

void freeMorada(morada* const m) {
    freeN(m->morada);
}

int morada_eCPValido (const uint8_t* const CP) {
    for(int i = 0; i < 7; ++i)
        if(!isdigit(CP[i]))
            return 0;

    return 1;
}

morada morada_dup (const morada m) {
    morada mcp = (morada) {
        .morada = strdup(m.morada)
    };
    memcpy(&(mcp.codigoPostal[0]), &(m.codigoPostal[0]), 7);
    return mcp;
}

/*
    - morada endereco
        - str morada
        * uint8_t codigoPostal[7]
*/
int save_morada (FILE* const f, const morada* const data) {
    int written = 0;
    written += save_str(f, data->morada);
    if(!data->morada) {
        menu_printError("ao gravar morada - morada inválida.");
        written = 0;
    }
    written += fwrite(data->codigoPostal, sizeof(uint8_t), 7, f);
    return written == 8;
}

int load_morada (FILE* const f, morada* const data) {
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