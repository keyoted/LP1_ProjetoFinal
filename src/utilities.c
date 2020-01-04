/**
 * @file    utilities.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Algumas utilidades comuns.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#include "utilities.h"

/**
 * @brief   Duplica uma string.
 * @param s String a ser duplicada.
 * @returns Uma cópia da string ou NULL casso o input da função seja NULL.
 */
char* strdup(const char* const s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1; // Make room for '\0'
    char* new;
    protectVarFcnCall(new, malloc(len), "strdup - alocação de memória recusada.");

    memcpy(new, s, len);
    return new;
}

/**
 * @brief          Pode ser utilizado como um iterador, imprime uma string.
 * @param item     String a ser impressa.
 * @param userdata Deve ser inicializado como 0, no final irá conter o número de
 *                 strings impressas.
 * @returns        0
 */
int vecPrintItemPredicate(char* const item, size_t* const userdata) {
    printf("   %*lu   |   %s\n", 8, (*userdata)++, protectStr(item));
    return 0;
}

/**
 * @brief       Responsável por salvar uma string num ficheiro.
 * @param f     Ficheiro onde salvar a string.
 * @param data  String para salvar no ficheiro 'f'.
 * @returns     0 se falhou ao salvar a string.
 * @returns     1 se slvou a string com sucesso.
 */
int save_str(FILE* const f, const char* const data) {
    if (!data) {
        const uint32_t ZERO = 0;
        fwrite(&ZERO, sizeof(uint32_t), 1, f);
        return 1;
    }
    uint32_t written = 0;
    uint32_t size    = strlen(data);
    written += fwrite(&size, sizeof(uint32_t), 1, f);
    written += fwrite(data, sizeof(uint8_t), size, f);
    return written == (size + 1);
}

/**
 * @brief       Responsavél por carregar uma string de um ficheiro.
 * @param f     Ficheiro de onde careggar a string.
 * @param data  String para salvar os dados carregados do ficheiro 'f'.
 * @returns     0 se falhou ao carregar a string.
 * @returns     1 se carregou a string com sucesso.
 */
int load_str(FILE* const f, char** const data) {
    uint32_t written = 0;
    uint32_t size    = 0;
    written += fread(&size, sizeof(uint32_t), 1, f);
    if (size == 0) {
        *data = NULL;
        return 1;
    }
    protectVarFcnCall(*data, malloc(size), "load_str - alocação de memória recusada.");
    written += fread(*data, sizeof(uint8_t), size, f);
    return written == (size + 1);
}