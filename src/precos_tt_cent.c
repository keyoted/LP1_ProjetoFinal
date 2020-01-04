/**
 * @file    precos_tt_cent.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Capaz de capturar os preços atuais que a empresa transportadora
 * fixou. Os preços são fixados em cêntimos.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#include "precos_tt_cent.h"

/**
 * @brief       Responsável por salvar preços num ficheiro.
 * @param f     Ficheiro onde salvar os preços.
 * @param data  Preços para salvar no ficheiro 'f'.
 * @returns     0 se falhou ao salvar os preços.
 * @returns     1 se slvou os preços com sucesso.
 */
int save_precos(FILE* const f, const precos_tt_cent* const data) {
    size_t written = 0;
    written += fwrite(&(data->REGULAR), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->URGENTE), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->VOLUMOSO), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->FRAGIL), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->PESADO), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->POR_KM), sizeof(uint64_t), 1, f);
    for (int org = 0; org < 10; ++org) { written += fwrite(data->MULT_CP[org], sizeof(_Float32), 10, f); }
    return written == 106;
}

/**
 * @brief       Responsavél por carregar preços de um ficheiro.
 * @param f     Ficheiro de onde careggar os preços.
 * @param data  Preços para salvar os dados carregados do ficheiro 'f'.
 * @returns     0 se falhou ao carregar os preços.
 * @returns     1 se carregou os preços com sucesso.
 */
int load_precos(FILE* const f, precos_tt_cent* const data) {
    size_t written = 0;
    written += fread(&(data->REGULAR), sizeof(uint64_t), 1, f);
    written += fread(&(data->URGENTE), sizeof(uint64_t), 1, f);
    written += fread(&(data->VOLUMOSO), sizeof(uint64_t), 1, f);
    written += fread(&(data->FRAGIL), sizeof(uint64_t), 1, f);
    written += fread(&(data->PESADO), sizeof(uint64_t), 1, f);
    written += fread(&(data->POR_KM), sizeof(uint64_t), 1, f);
    for (int org = 0; org < 10; ++org) { written += fread(data->MULT_CP[org], sizeof(_Float32), 10, f); }
    return written == 106;
}