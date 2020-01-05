/**
 * @file    menu.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   API simples para mostrar informação e capturar input do utilizador.
 * @version 1
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "menu.h"

#include "utilities.h"

/**
 * @brief Função auxiliar para limpar o buffer de input.
 */
void cleanInputBuffer() {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

/**
 * @brief     Remove os espaços extra no início e final da string de input.
 * @param str String para ser reduzida.
 * @returns   Um ponteiro para a string reduzida.
 * @returns   NULL caso a string seja apenas constituida por espaços.
 * @warning   A string de retorno é uma substring da string de input, caso esta
 *            seja dealocada, a substring também será dealocada.
 */
char* subStringTrimWhiteSpace(char* str) {
    if (!str) return NULL;

    // At the beguining
    while (isspace(*str)) str++;
    if (*str == '\0') // All spaces?
        return NULL;

    // At the end
    char* end = &str[strlen(str) - 1];
    while (end > str && isspace(*end)) end--;
    end[1] = '\0';

    return str;
}

/**
 * @brief   Lê uma string do standard input.
 * @returns O ponteiro para uma string sem espaços extra no início ou no fim.
 * @returns NULL caso só tenham sido introduzidos espaços.
 * @warning Pode retornar NULL.
 */
char* menu_readString() {
    const int STEP     = 16;
    size_t    alocated = STEP;
    size_t    size     = 0;
    char*     str;
    protectVarFcnCall(str, malloc(STEP), "menu_readString - alocação de memória recusada.");

    int ch;
    printf(" $ ");
    while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
        str[size++] = ch;
        if (size == alocated) {
            char* tmp = realloc(str, alocated += STEP);
            if (!tmp) {
                --size;
                menu_printError("menu_readString - realocação de memória recusada.");
                break;
            }
            str = tmp;
        }
    }
    str[size++] = '\0';

    char* trimed = strdup(subStringTrimWhiteSpace(str));
    free(str);
    return trimed;
}

/**
 * @brief   Liberta X e lê uma string vâlida (não nula e não só espaços),
 *          guardando-a em X.
 * @param X O ponteiro que será libertado e terá a string final.
 */
void menu_readNotNulStr(char** X) {
    freeN(*X);
    while (!(*X)) { *X = menu_readString(); }
}

/**
 * @brief       Lê uma float do standard input.
 * @param value Valor para onde será guardado o valor.
 */
void menu_read_Float32(_Float32* const value) {
    float f;
    while (1) {
        printf(" $ ");
        if (scanf("%f", &f) != 1) {
            menu_printError("Não foi inserido um número válido.");
            cleanInputBuffer();
            continue;
        }
        cleanInputBuffer();
        *value = (_Float32) f;
        return;
    }
}

/**
 * @brief       Lê uma int do standard input.
 * @param value Valor para onde será guardado o valor.
 */
void menu_readInt(int* const value) {
    while (1) {
        printf(" $ ");
        if (scanf("%d", value) != 1) {
            menu_printError("Não foi inserido um número válido.");
            cleanInputBuffer();
            continue;
        }
        cleanInputBuffer();
        return;
    }
}

/**
 * @brief     Lê uma int do standard input entre [min, max]
 * @param min Valor minimo da int para ser lida.
 * @param max Valor maximo da int para ser lida.
 * @param op  Valor para onde será guardado o valor.
 */
void menu_readIntMinMax(const int min, const int max, int* const op) {
    printf("Insira um numero entre [%d e %d]", min, max);
    while (1) {
        menu_readInt(op);
        if (*op >= min) {
            if (*op <= max) {
                return;
            } else
                menu_printError("[%d] é maior que [%d].", *op, max);
        } else
            menu_printError("[%d] é menor que [%d].", *op, min);
    }
}

/**
 * @brief       Imprime um menu deixando o utilizador selecionar uma das opções.
 * @param itens Vetor se strings que constituem as opções que o utilizador pode
 *              selecionar.
 * @returns     Um valor entre [-1, itens.size[ correspondente à opção
 *              selecionada ou "Sair" para -1
 */
int menu_selection(const strvec* const itens) {
    int op  = -2;
    int max = itens->size;
    while (op == -2) {
        printf("   Opção      |   Item\n");
        printf("         -2   |   Reimprimir\n");
        printf("         -1   |   Sair\n");
        size_t i = 0;
        strvec_iterateFW((strvec*) itens, (strvec_predicate_t) &vecPrintItemPredicate, &i);
        menu_readIntMinMax(-2, max - 1, &op);
        menu_printDiv();
    }
    return op;
}

/**
 * @brief Imprime uma divisória.
 */
void menu_printDiv() {
    printf("-------------------------------------------------------------------"
           "-------------\n");
}

/**
 * @brief     Utilizado para imprimir erros.
 * @param err O formato do erro.
 * @param ... Argumentos extra.
 */
void menu_printError(const char* const err, ...) {
    va_list args;
    printf("*** ERRO: ");
    va_start(args, err);
    vprintf(err, args);
    va_end(args);
    printf("\n");
}

/**
 * @brief      Utilizado para imprimir informação.
 * @param info O formato da informação.
 * @param ...  Argumentos extra.
 */
void menu_printInfo(const char* const info, ...) {
    va_list args;
    printf("*** INFO: ");
    va_start(args, info);
    vprintf(info, args);
    va_end(args);
    printf("\n");
}

/**
 * @brief        Utilizado para imprimir um título.
 * @param header String para o header.
 */
void menu_printHeader(const char* header) {
    if (!header) header = "";
    int spacesize = (80 - strlen(header)) - 8;
    if (spacesize < 3) spacesize = 3;
    for (int i = 0; i < spacesize / 2; i++) printf(" ");
    printf("*** ");
    printf("%.66s", header);
    printf(" ***\n");
};

/**
 * @brief    Imprime informação breve sobre a encomenda.
 * @param e  Encomenda a ser impressa.
 * @param uv Vetor de utilizadores ao qual o ID de utilizador da encomenda faz
 *           referência.
 */
void menu_printEncomendaBrief(const encomenda* const e, const utilizadorvec* const uv) {
    if (e->tipoEstado & ENCOMENDA_TIPO_URGENTE)
        printf("URGENTE ");
    else
        printf("REGULAR ");

    switch (e->tipoEstado & 0xF0) {
        case ENCOMENDA_ESTADO_EXPEDIDA: printf("EXPEDIDA  "); break;
        case ENCOMENDA_ESTADO_CANCELADA: printf("CANCELADA "); break;
        case ENCOMENDA_ESTADO_EM_ENTREGA: printf("EM ENTREGA"); break;
        case ENCOMENDA_ESTADO_ENTREGUE: printf("ENTREGUE  "); break;
    }

    printf(" (%.9s) ", uv->data[e->ID_cliente].NIF);
    struct tm* lt = localtime(&e->criacao);
    printf(" %d/%d/%d %d:%d  -  ", 1900 + lt->tm_year, 1 + lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min);
    printf("%luc", encomenda_CalcPreco(e));
}

/**
 * @brief   Imprime informação sobre o utilizador.
 * @param u Utilizador para ser impresso.
 */
void menu_printUtilizador(const utilizador u) {
    printf("%s ", protectStr(u.nome));
    printf("(%.9s) ", u.NIF);
    switch (u.tipo) {
        case UTILIZADOR_CLIENTE: printf("CLIENTE"); break;
        case UTILIZADOR_DESATIVADO: printf("DESATIVADO"); break;
        case UTILIZADOR_DIRETOR: printf("DIRETOR"); break;
    }
}

/**
 * @brief   Imprime informação sobre o artigo.
 * @param a Artigo para ser impresso.
 */
void menu_printArtigo(const artigo* const a) {
    // nome gramas cm2 tratamento especial
    printf("%s  -   %lug   %lucm2   * %s", protectStr(a->nome), a->peso_gramas, a->cmCubicos,
           protectStr(a->tratamentoEspecial));
}

/**
 * @brief    Imprime informação detalhada sobre a encomenda.
 * @param e  Encomenda a ser impressa.
 * @param uv Vetor de utilizadores ao qual o ID de utilizador da encomenda faz
 *           referência.
 */
void menu_printEncomendaDetail(const encomenda* const e, const utilizadorvec* const uv) {
    menu_printDiv();
    menu_printDiv();
    menu_printHeader("Recibo de Encomenda");
    printf("*** NIF do Cliente: %.9s\n", uv->data[e->ID_cliente].NIF);

    struct tm* lt = localtime(&e->criacao);
    printf("Data de criação: %d/%d/%d %d:%d", 1900 + lt->tm_year, 1 + lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min);

    menu_printHeader("Origem");
    printf("*** Morada: %s\n", protectStr(e->origem.morada));
    printf("*** Código Postal: %.4s-%.3s\n", e->origem.codigoPostal, &e->origem.codigoPostal[4]);

    menu_printHeader("Destino");
    printf("*** Morada: %s\n", protectStr(e->destino.morada));
    printf("*** Código Postal: %.4s-%.3s\n", e->destino.codigoPostal, &e->destino.codigoPostal[4]);


    menu_printHeader("Artigos");
    uint64_t pt = 0;
    uint64_t vt = 0;
    printf("*       ID | NOME                          | PESO     | VOLUME   | "
           "PESO T.  | VOL. T.  | * TRATAMENTO ESPECIAL\n");
    for (size_t i = 0; i < e->artigos.size; ++i) {
        pt += e->artigos.data[i].peso_gramas;
        vt += e->artigos.data[i].cmCubicos;
        printf("* %*lu |", 8, i + 1);
        printf(" %29.29s |", protectStr(e->artigos.data[i].nome));
        printf(" %*lu |", 8, e->artigos.data[i].peso_gramas);
        printf(" %*lu |", 8, e->artigos.data[i].cmCubicos);
        printf(" %*lu |", 8, pt);
        printf(" %*lu |", 8, vt);
        printf(" %s\n", protectStr(e->artigos.data[i].tratamentoEspecial));
    }

    menu_printHeader("Outras Informações");
    printf("*** ESTADO: ");
    switch (e->tipoEstado & 0xF0) {
        case ENCOMENDA_ESTADO_EXPEDIDA: printf("EXPEDIDA\n"); break;
        case ENCOMENDA_ESTADO_CANCELADA: printf("CANCELADA\n"); break;
        case ENCOMENDA_ESTADO_EM_ENTREGA: printf("EM ENTREGA\n"); break;
        case ENCOMENDA_ESTADO_ENTREGUE: printf("ENTREGUE\n"); break;
        default: printf("DESCONHECIDO\n");
    }

    printf("*** TIPO:        | ");
    if (e->tipoEstado & ENCOMENDA_TIPO_URGENTE)
        printf("URGENTE    | ");
    else
        printf("REGULAR    | ");
    if (e->tipoEstado & ENCOMENDA_TIPO_FRAGIL)
        printf("FRAGIL     | ");
    else
        printf("RESISTENTE | ");
    if (e->tipoEstado & ENCOMENDA_TIPO_PESADO)
        printf("PESADO     | ");
    else
        printf("LEVE       | ");
    if (e->tipoEstado & ENCOMENDA_TIPO_VOLUMOSO)
        printf("VOLUMOSO   |\n");
    else
        printf("PEQUENO    |\n");

    uint64_t tpt = 0;
    printf("*** TIPO PREÇOS: | ");
    if (e->tipoEstado & ENCOMENDA_TIPO_URGENTE) {
        printf(" %*luc | ", 8, e->precos.URGENTE);
        tpt += e->precos.URGENTE;
    } else {
        printf(" %*luc | ", 8, e->precos.REGULAR);
        tpt += e->precos.REGULAR;
    }
    if (e->tipoEstado & ENCOMENDA_TIPO_FRAGIL) {
        printf(" %*luc | ", 8, e->precos.FRAGIL);
        tpt += e->precos.FRAGIL;
    } else
        printf(" %*luc | ", 8, (uint64_t) 0);
    if (e->tipoEstado & ENCOMENDA_TIPO_PESADO) {
        printf(" %*luc | ", 8, e->precos.PESADO);
        tpt += e->precos.PESADO;
    } else
        printf(" %*luc | ", 8, (uint64_t) 0);
    if (e->tipoEstado & ENCOMENDA_TIPO_VOLUMOSO) {
        printf(" %*luc |\n", 8, e->precos.VOLUMOSO);
        tpt += e->precos.VOLUMOSO;
    } else
        printf(" %*luc |\n", 8, (uint64_t) 0);

    printf("*** Preçco base: %luc\n", tpt);
    printf("*** Distancia: %luKm\n", e->distancia_km);
    printf("*** Preço Por Km: %luc\n", e->precos.POR_KM);
    if (e->origem.codigoPostal[0] > '9' || e->origem.codigoPostal[0] < '0' || e->destino.codigoPostal[0] > '9' ||
        e->destino.codigoPostal[0] < '0') {
        printf("*** Multiplicador de Código Postal: ERRO\n");
        printf("*** Preço Final em Cêntimos: ERRO\n");
    } else {
        const _Float32 multcp = e->precos.MULT_CP[e->origem.codigoPostal[0] - '0'][e->destino.codigoPostal[0] - '0'];
        printf("*** Multiplicador de Código Postal: %f\n", (double) multcp);
        printf("*** Preço Final em Cêntimos: %luc\n", encomenda_CalcPreco(e));
    }
    menu_printDiv();
}

/**
 * @brief       Imprime o recibo mensal para um certo utilizador. Apenas são
 * impressas no recibo encomendas que já foram entregues.
 *
 * @param ID_U  ID do utilizador para quem imprimir o recibo.
 * @param mes   Mês do recibo. [1, 12]
 * @param ano   Ano do recibo.
 * @param e     Vetor das encomendas.
 * @param uv    Vetor dos utilizadores.
 */
void menu_printReciboMensal(const uint64_t ID_U, int mes, int ano, const encomendavec* const e,
                            const utilizadorvec* const uv) {
    size_t tot_enc   = 0;
    size_t tot_art   = 0;
    size_t tot_preco = 0;

    ano -= 1900;
    mes -= 1;

    menu_printDiv();
    printf("*** Nome: %s\n", protectStr(uv->data[ID_U].nome));
    printf("*** NIF: %.9s\n\n", uv->data[ID_U].NIF);

    for (size_t i = 0; i < e->size; ++i) {
        struct tm* enctm = localtime(&e->data[i].criacao);
        if (enctm->tm_year != ano || enctm->tm_mon != mes || ID_U != e->data[i].ID_cliente) continue;
        encomenda* atual = &e->data[i];
        if (atual->tipoEstado != ENCOMENDA_ESTADO_ENTREGUE) continue;
        menu_printEncomendaBrief(atual, uv);
        printf("\n");
        size_t art;
        for (art = 0; art < atual->artigos.size; ++art) {
            printf("\t");
            menu_printArtigo(&atual->artigos.data[art]);
            printf("\n");
        }
        tot_art += art;
        ++tot_enc;
        tot_preco += encomenda_CalcPreco(atual);
    }
    printf("\n");
    ano += 1900;
    mes += 1;
    printf("*** Encomendas feitas (%d/%d):    %lu\n", ano, mes, tot_enc);
    printf("*** Artigos encomendados (%d/%d): %lu\n", ano, mes, tot_art);
    printf("*** Preço final do mês (%d/%d):   %luc\n", ano, mes, tot_preco);
}