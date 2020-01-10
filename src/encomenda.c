/**
 * @file    encomenda.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Uma encomenda é um conjunto de artigos transportados de um certo
 *          adereço de origem para um adereço de destino.
 *          A encomenda percorre uma certa distância e tem diversos estados
 *          como ENCOMENDA_ESTADO_EM_ENTREGA ou tipos como
 *          ENCOMENDA_TIPO_URGENTE.
 *          Adicionalmente a encomenda armazena a tabela de preços ativa quando
 *          esta foi criada, a data de criação e o ID do cliente que criou a
 *          encomenda.
 * @version 0.1
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 */

#include "encomenda.h"

#include "menu.h"
#include "utilities.h"

/**
 * @brief           Inicializador para encomendas.
 * @returns         Uma encomenda, válida mas sem artigos.
 */
encomenda newEncomenda() {
    return (encomenda) {
        .artigos = artigovec_new(), .origem = newMorada(), .destino = newMorada(), .criacao = time(NULL)};
}

/**
 * @brief           Responsavél por libertar a memória da encomenda.
 * @param e         Encomenda para ser libertado.
 */
void freeEncomenda(encomenda* const e) {
    artigovec_free(&e->artigos);
    freeMorada(&e->origem);
    freeMorada(&e->destino);
}

/**
 * @brief           Calcula se o peso de uma encomenda a faz ser considerada
 *                  pesada.
 * @param a         Peso da encomenda.
 * @returns         0 se a encomenda é considerada leve.
 * @returns         1 se a encomenda é considerada pesada.
 */
int encomenda_ePesado(const uint64_t a) { return a > 20000; }

/**
 * @brief           Calcula se o volume de uma encomenda a faz ser considerada
 *                  volumosa.
 * @param a         Volume da encomenda.
 * @returns         0 se a encomenda é considerada pequena.
 * @returns         1 se a encomenda é considerada volumosa.
 */
int encomenda_eVolumoso(const uint64_t a) { return a > (50 * 50 * 50); }

/**
 * @brief           Calcula o preço de uma encomenda, em cêntimos.
 * @param e         Encomenda cujo preço será calculado.
 * @returns         O preço da encomenda em cêntimos.
 * @returns         0 se a encomenda tiver um código postal inválido.
 * @warning         O tipoEstado da encomenda tem que estar correto antes de
 *                  chamar esta função.
 */
uint64_t encomenda_CalcPreco(const encomenda* const e) {
    uint64_t precoFinal;
    // Soma dos tipos de transporte
    // ENCOMENDA_TIPO_URGENTE
    if (e->tipoEstado & ENCOMENDA_TIPO_URGENTE)
        precoFinal = e->precos.URGENTE;
    else
        precoFinal = e->precos.REGULAR;
    // ENCOMENDA_TIPO_FRAGIL
    if (e->tipoEstado & ENCOMENDA_TIPO_FRAGIL) precoFinal += e->precos.FRAGIL;
    // ENCOMENDA_TIPO_PESADO
    if (e->tipoEstado & ENCOMENDA_TIPO_PESADO) precoFinal += e->precos.PESADO;
    // ENCOMENDA_TIPO_VOLUMOSO
    if (e->tipoEstado & ENCOMENDA_TIPO_VOLUMOSO) precoFinal += e->precos.VOLUMOSO;

    // Final do cálculo
    if (e->origem.codigoPostal[0] > '9' || e->origem.codigoPostal[0] < '1' || e->destino.codigoPostal[0] > '9' ||
        e->destino.codigoPostal[0] < '1') {
        printf("\n");
        menu_printError("ao calcular preço da encomenda - codigo postal defeituoso.");
        return 0;
    }
    const _Float32 multcp = e->precos.MULT_CP[e->origem.codigoPostal[0] - '1'][e->destino.codigoPostal[0] - '1'];
    return (precoFinal + e->distancia_km * e->precos.POR_KM) * multcp;
}

/**
 * @brief           Torna uma encomenda urgente, se já for urgente é tornada
 *                  regular.
 * @param e         A encomenda para ser alterada.
 */
void encomenda_TIPO_URGENTE(encomenda* const e) {
    if (e->tipoEstado & ENCOMENDA_TIPO_URGENTE) {
        e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_URGENTE);
        menu_printInfo("encomenda definida como REGULAR.");
    } else {
        e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_URGENTE;
        menu_printInfo("encomenda definida como URGENTE.");
    }
}

/**
 * @brief           Procura se existe um tratamento especial num dos artigos da
 *                  encomenda, caso exista a encomenda é defenida como frágil.
 * @param e         A encomenda para ser alterada.
 */
void encomenda_TIPO_FRAGIL(encomenda* const e) {
    for (size_t i = 0; i < e->artigos.size; i++) {
        if (e->artigos.data[i].tratamentoEspecial) {
            e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_FRAGIL;
            menu_printInfo("encomenda definida como FRAGIL.");
            return;
        }
    }
    e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_FRAGIL);
    menu_printInfo("encomenda definida como RESISTENTE.");
}

/**
 * @brief           Caso a encomenda seja resistente, é definida como frágil.
 *                  Caso seja frágil e não tenha nenhum artigo com um tratamento
 *                  especial, é definida como resistente.
 * @param e         A encomenda para ser alterada.
 */
void encomenda_TIPO_FRAGIL_togle(encomenda* const e) {
    if (!(e->tipoEstado & ENCOMENDA_TIPO_FRAGIL)) {
        // Encomenda não é fragil, logo pode ser posta como fragil manualmente
        e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_FRAGIL;
        menu_printInfo("encomenda definida como FRAGIL.");
        return;
    }

    for (size_t i = 0; i < e->artigos.size; i++) {
        if (e->artigos.data[i].tratamentoEspecial) {
            // Encomenda é obigatoriamente fragil
            menu_printInfo("encomenda obrigatoriamente definida como FRAGIL.");
            return;
        }
    }
    // Encomenda pode ser defenida como resistente
    e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_FRAGIL);
    menu_printInfo("encomenda definida como RESISTENTE.");
}

/**
 * @brief           Define a encomenda como pesada ou leve de acordo com o peso
 *                  acumulado dos artigos da encomenda.
 * @param e         A encomenda para ser alterada.
 */
void encomenda_TIPO_PESADO(encomenda* const e) {
    uint64_t pesoAcumulado = 0;
    for (size_t i = 0; i < e->artigos.size; i++) { pesoAcumulado += e->artigos.data[i].peso_gramas; }
    if (encomenda_ePesado(pesoAcumulado)) {
        e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_PESADO;
        menu_printInfo("encomenda definida como PESADO.");
    } else {
        e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_PESADO);
        menu_printInfo("encomenda definida como LEVE.");
    }
}

/**
 * @brief           Define a encomenda como volumosa ou pequena de acordo com o
 *                  volume acumulado dos artigos da encomenda.
 * @param e         A encomenda para ser alterada.
 */
void encomenda_TIPO_VOLUMOSO(encomenda* const e) {
    uint64_t volumeAcumulado = 0;
    for (size_t i = 0; i < e->artigos.size; i++) { volumeAcumulado += e->artigos.data[i].cmCubicos; }
    if (encomenda_eVolumoso(volumeAcumulado)) {
        e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_VOLUMOSO;
        menu_printInfo("encomenda definida como VOLUMOSO.");
    } else {
        e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_VOLUMOSO);
        menu_printInfo("encomenda definida como PEQUENO.");
    }
}

/**
 * @brief           Define a encomenda como 'toggle' caso o seu estado seja
 *                  'paraMudar', caso contrário o estado é tornado 'paraMudar'.
 * @param e         A encomenda para ser alterada.
 * @param paraMudar Estado que o artigo vai tomar.
 * @param toggle    Estado que o artigo toma, caso já esteja 'paraMudar'.
 * @returns         1 caso o artigo estaja no estado 'paraMudar' antes de chamr
 *                  a função.
 * @returns         0 caso contrário.
 */
int encomenda_generic_estado(encomenda* const e, const uint8_t paraMudar, const uint8_t toggle) {
    if ((e->tipoEstado & 0xF0) == paraMudar) {
        // Se já estiver no estado para mudar, fazer toggle
        e->tipoEstado = (e->tipoEstado & 0x0F) | toggle;
        return 1;
    } else {
        e->tipoEstado = (e->tipoEstado & 0x0F) | paraMudar;
        return 0;
    }
}

/**
 * @brief           Tona o estado da encomenda para 'EM ENTREGA', caso a
 *                  encomenda já esteja 'EM ENTREGA' é tornada 'CANCELADA'.
 * @param e         A encomenda para ser alterada.
 */
void encomenda_ESTADO_EM_ENTREGA(encomenda* const e) {
    if (encomenda_generic_estado(e, ENCOMENDA_ESTADO_EM_ENTREGA, ENCOMENDA_ESTADO_CANCELADA)) {
        menu_printInfo("encomenda definida como CANCELADA.");
    } else {
        menu_printInfo("encomenda definida como EM ENTREGA.");
    }
}

/**
 * @brief           Tona o estado da encomenda para 'EXPEDIDA', caso a encomenda
 *                  já esteja 'EXPEDIDA' é tornada 'EM ENTREGA'.
 * @param e         A encomenda para ser alterada.
 */
void encomenda_ESTADO_EXPEDIDA(encomenda* const e) {
    if (encomenda_generic_estado(e, ENCOMENDA_ESTADO_EXPEDIDA, ENCOMENDA_ESTADO_EM_ENTREGA)) {
        menu_printInfo("encomenda definida como EM ENTREGA.");
    } else {
        menu_printInfo("encomenda definida como EXPEDIDA.");
    }
}


/**
 * @brief           Tona o estado da encomenda para 'ENTREGUE', caso a encomenda
 *                  já esteja 'ENTREGUE' é tornada 'EM ENTREGA'.
 * @param e         A encomenda para ser alterada.
 */
void encomenda_ESTADO_ENTREGUE(encomenda* const e) {
    if (encomenda_generic_estado(e, ENCOMENDA_ESTADO_ENTREGUE, ENCOMENDA_ESTADO_EM_ENTREGA)) {
        menu_printInfo("encomenda definida como EM ENTREGA.");
    } else {
        menu_printInfo("encomenda definida como ENTREGUE.");
    }
}

/**
 * @brief           Tona o estado da encomenda para 'CANCELADA', caso a
 *                  encomenda já esteja 'CANCELADA' é tornada 'EM ENTREGA'.
 * @param e         A encomenda para ser alterada.
 */
void encomenda_ESTADO_CANCELADA(encomenda* const e) {
    if (encomenda_generic_estado(e, ENCOMENDA_ESTADO_CANCELADA, ENCOMENDA_ESTADO_EM_ENTREGA)) {
        menu_printInfo("encomenda definida como EM ENTREGA.");
    } else {
        menu_printInfo("encomenda definida como CANCELADA.");
    }
}

/**
 * @brief            Formaliza uma encomenda, definindo corretamente todos os
 *                   seus parametros.
 * @param artigos    Os artigos da encomenda. (transfere o domínio)
 * @param precos     Tabela de preços atual.
 * @param ID_cliente ID do cliente que formalizou a encomenda.
 * @param org        A morada de origem da encomenda. (transfere o domínio)
 * @param dest       A morada de destino da encomenda. (transfere o domínio)
 * @param dist       A distância que a encomenda irá precorrer.
 * @returns          A encomenda formalizada, com todos os parametros
 *                   corretamente definidos, incluindo 'tipoEstado' e 'criacao'.
 */
encomenda encomenda_formalizar(const artigovec artigos, const precos_cent precos, const uint64_t ID_cliente,
                               const morada org, const morada dest, const uint64_t dist) {
    encomenda e;
    e.artigos      = artigos;
    e.destino      = dest;
    e.distancia_km = dist;
    e.ID_cliente   = ID_cliente;
    e.origem       = org;
    e.precos       = precos;
    e.tipoEstado   = ENCOMENDA_ESTADO_EM_ENTREGA;
    e.criacao      = time(NULL);
    for (int i = 0; i < 9; i++) { memcpy(&e.precos.MULT_CP[i], &precos.MULT_CP[i], 9); }
    encomenda_TIPO_VOLUMOSO(&e);
    encomenda_TIPO_FRAGIL(&e);
    encomenda_TIPO_PESADO(&e);
    return e;
}

/**
 * @brief           Responsável por salvar uma encomenda num ficheiro.
 * @param f         Ficheiro onde salvar a encomenda.
 * @param data      Encomenda para salvar no ficheiro 'f'.
 * @returns         0 se falhou ao salvar a encomenda.
 * @returns         1 se slvou a encomenda com sucesso.
 */
int save_encomenda(FILE* const f, const encomenda* const data) {
    // Gravar morada
    if (!artigovec_write(&(data->artigos), f)) {
        menu_printError("ao gravar encomenda - artigovec_write falhou.");
        return 0;
    }
    // Gravar moradas
    if (!save_morada(f, &data->origem)) {
        menu_printError("ao gravar encomenda - save_morada falhou.");
        return 0;
    }
    if (!save_morada(f, &data->destino)) {
        menu_printError("ao gravar encomenda - save_morada falhou.");
        return 0;
    }
    // Gravar preços
    if (!save_precos(f, &data->precos)) {
        menu_printError("ao gravar encomenda - save_precos falhou.");
        return 0;
    }
    // Gravar o resto
    size_t written = 0;
    written += fwrite(&data->distancia_km, sizeof(uint64_t), 1, f);
    written += fwrite(&data->tipoEstado, sizeof(uint8_t), 1, f);
    written += fwrite(&data->ID_cliente, sizeof(uint64_t), 1, f);
    written += fwrite(&data->criacao, sizeof(time_t), 1, f);
    return written == 4;
}

/**
 * @brief           Responsavél por carregar uma encomenda de um ficheiro.
 * @param f         Ficheiro de onde careggar a encomenda.
 * @param data      Encomenda para salvar os dados carregados do ficheiro 'f'.
 * @returns         0 se falhou ao carregar a encomenda.
 * @returns         1 se carregou a encomenda com sucesso.
 */
int load_encomenda(FILE* const f, encomenda* const data) {
    // Carregar artigos
    data->artigos = artigovec_new();
    if (!artigovec_read(&(data->artigos), f)) {
        menu_printError("ao carregar encomenda - artigovec_read falhou.");
        return 0;
    }
    // Carregar morada
    if (!load_morada(f, &data->origem)) {
        menu_printError("ao carregar encomenda - load_morada falhou.");
        return 0;
    }
    if (!load_morada(f, &data->destino)) {
        menu_printError("ao carregar encomenda - load_morada falhou.");
        return 0;
    }
    // Carregar preços
    if (!load_precos(f, &data->precos)) {
        menu_printError("ao carregar encomenda - load_precos falhou.");
        return 0;
    }
    // Carregar o resto
    size_t written = 0;
    written += fread(&data->distancia_km, sizeof(uint64_t), 1, f);
    written += fread(&data->tipoEstado, sizeof(uint8_t), 1, f);
    written += fread(&data->ID_cliente, sizeof(uint64_t), 1, f);
    written += fread(&data->criacao, sizeof(time_t), 1, f);
    return written == 4;
}
