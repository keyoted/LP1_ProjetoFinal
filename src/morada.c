/**
 * @file    morada.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Estrutura que é capaz de gravar a localização de um utilizador.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#include "morada.h"

#include "menu.h"
#include "utilities.h"

/**
 * @brief   Cria uma nova morada.
 * @returns Uma morada válida.
 */
morada newMorada() { return (morada) {.morada = strdup("Rua")}; }

/**
 * @brief   Dealoca memória de uma morada.
 * @param m Morada para ser dealocada.
 */
void freeMorada(morada* const m) { freeN(m->morada); }

/**
 * @brief    Responsável por verificar se um código postal é válido.
 * @param CP Código postal para ser validado.
 * @returns  0 se o código postal é inválido.
 * @returns  1 se o código postal é válido.
 */
int morada_eCPValido(const uint8_t* const CP) {
    for (int i = 0; i < 7; ++i)
        if (!isdigit(CP[i])) return 0;

    return 1;
}

/**
 * @brief   Duplica uma morada.
 * @param m Morada a ser duplicada.
 * @returns Uma cópia da morada 'm'.
 */
morada morada_dup(const morada m) {
    morada mcp = (morada) {.morada = strdup(m.morada)};
    memcpy(&(mcp.codigoPostal[0]), &(m.codigoPostal[0]), 7);
    return mcp;
}

/**
 * @brief       Responsável por salvar uma morada num ficheiro.
 * @param f     Ficheiro onde salvar a morada.
 * @param data  Morada para salvar no ficheiro 'f'.
 * @returns     0 se falhou ao salvar a morada.
 * @returns     1 se slvou a morada com sucesso.
 */
int save_morada(FILE* const f, const morada* const data) {
    int written = 0;
    written += save_str(f, data->morada);
    if (!data->morada) {
        menu_printError("ao gravar morada - morada inválida.");
        written = 0;
    }
    written += fwrite(data->codigoPostal, sizeof(uint8_t), 7, f);
    return written == 8;
}

/**
 * @brief       Responsavél por carregar uma morada de um ficheiro.
 * @param f     Ficheiro de onde careggar a morada.
 * @param data  Artigo para salvar os dados carregados do ficheiro 'f'.
 * @returns     0 se falhou ao carregar a morada.
 * @returns     1 se carregou a morada com sucesso.
 */
int load_morada(FILE* const f, morada* const data) {
    int written = 0;
    written += load_str(f, &(data->morada));
    if (!data->morada) {
        menu_printError("ao carregar morada - morada inválida.");
        data->morada = strdup("Inválido");
        written      = 0;
    }
    written += fread(data->codigoPostal, sizeof(uint8_t), 7, f);
    return written == 8;
}