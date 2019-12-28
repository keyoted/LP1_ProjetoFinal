#include "precos_tt_cent.h"

/*
    - precos_tt_cent tabela_de_precos
        * uint64_t REGULAR
        * uint64_t URGENTE
        * uint64_t VOLUMOSO
        * uint64_t FRAGIL
        * uint64_t PESADO
        * uint64_t POR_KM
        - _Float32[100] MULT_CP
            * _Float32 (32b) de [0][0], [0][1] .. [0][9], [1][0] .. [9][9]
*/
int save_precos (FILE* f, precos_tt_cent* data) {
    size_t written = 0;
    written += fwrite(&(data->REGULAR ), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->URGENTE ), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->VOLUMOSO), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->FRAGIL  ), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->PESADO  ), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->POR_KM  ), sizeof(uint64_t), 1, f);
    for(int org = 0; org < 10; ++org) {
        written += fwrite(data->MULT_CP[org], sizeof(_Float32), 10, f);
    }
    return written == 106;
}

int load_precos (FILE* f, precos_tt_cent* data) {
    size_t written = 0;
    written += fread(&(data->REGULAR ), sizeof(uint64_t), 1, f);
    written += fread(&(data->URGENTE ), sizeof(uint64_t), 1, f);
    written += fread(&(data->VOLUMOSO), sizeof(uint64_t), 1, f);
    written += fread(&(data->FRAGIL  ), sizeof(uint64_t), 1, f);
    written += fread(&(data->PESADO  ), sizeof(uint64_t), 1, f);
    written += fread(&(data->POR_KM  ), sizeof(uint64_t), 1, f);
    for(int org = 0; org < 10; ++org) {
        written += fread(data->MULT_CP[org], sizeof(_Float32), 10, f);
    }
    return written == 106;
}