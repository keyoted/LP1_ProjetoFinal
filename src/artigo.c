/**
 * @file    artigo.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Um artigo é um objeto com um nome, peso e volume que pode ou não
 *          requerer tratamento especial.
 * @version 1
 * @date    2019-12-31
 * @copyright Copyright (c) 2019
 */

#include "artigo.h"

#include "menu.h"
#include "utilities.h"

/**
 * @brief       Inicializador para artigos.
 * @returns     Um novo artigo válido.
 */
artigo newArtigo() {
    return (artigo) {
        .nome               = strdup("Artigo"),
        .tratamentoEspecial = NULL,
    };
}

/**
 * @brief       Responsavél por libertar a memória do artigo.
 * @param a     Artigo para ser libertado.
 */
void freeArtigo(artigo* const a) {
    freeN(a->nome);
    freeN(a->tratamentoEspecial);
}

/**
 * @brief       Responsável por salvar um artigo num ficheiro.
 * @param f     Ficheiro onde salvar o artigo.
 * @param data  Artigo para salvar no ficheiro 'f'.
 * @returns     0 se falhou ao salvar o artigo.
 * @returns     1 se slvou o artigo com sucesso.
 */
int save_artigo(FILE* const f, const artigo* const data) {
    int written = 0;
    written += save_str(f, data->nome);
    if (!data->nome) {
        menu_printError("ao gravar artigo - nome inválido.");
        written = 0;
    }
    written += save_str(f, data->tratamentoEspecial);
    written += fwrite(&(data->peso_gramas), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->cmCubicos), sizeof(uint64_t), 1, f);
    return written == 4;
}

/**
 * @brief       Responsavél por carregar um artigo de um ficheiro.
 * @param f     Ficheiro de onde careggar o artigo.
 * @param data  Artigo para salvar os dados carregados do ficheiro 'f'.
 * @returns     0 se falhou ao carregar o artigo.
 * @returns     1 se carregou o artigo com sucesso.
 */
int load_artigo(FILE* const f, artigo* const data) {
    int written = 0;
    written += load_str(f, &(data->nome));
    if (!data->nome) {
        menu_printError("ao carregar artigo - nome inválido.");
        data->nome = strdup("Inválido");
        written    = 0;
    }
    written += load_str(f, &(data->tratamentoEspecial));
    written += fread(&(data->peso_gramas), sizeof(uint64_t), 1, f);
    written += fread(&(data->cmCubicos), sizeof(uint64_t), 1, f);
    return written == 4;
}
