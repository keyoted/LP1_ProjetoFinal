/**
 * @file    outrasListagens.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   As 5 listagens extra propostas pelos alunos.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#include "outrasListagens.h"

#include "menu.h"
#include "utilities.h"

/**
 * @brief Imprime encomendas formalizadas num certo periodo de tempo.
 */
void listagem_Encomendas_Periodo_de_Tempo() {
    menu_printDiv();
    menu_printHeader("Listagem de Encomendas em Espaço de Tempo");

    time_t     now   = time(NULL);
    struct tm* tmnow = localtime(&now);

    int mesi = 0;
    int mesf = 0;
    int anoi = 0;
    int anof = 0;

    printf("Introduzir mês de inicio para a pesquisa");
    menu_readIntMinMax(1, 12, &mesi);

    printf("Introduzir ano de inicio para a pesquisa");
    menu_readIntMinMax(1900, tmnow->tm_year + 1900, &anoi);

    printf("Introduzir mês de fim para a pesquisa");
    menu_readIntMinMax(1, 12, &mesf);

    printf("Introduzir ano de fim para a pesquisa");
    menu_readIntMinMax(1900, tmnow->tm_year + 1900, &anof);

    // Garantir que a data de início é mais "antiga" que a de fim.
    // 0 --- inicio --- fim --- +infinito
    // inicio < fim
    if (anoi > anof) {
        // Se o ano de inicio for mais "recente"
        // Trocar ano e mes
        int tmp = anoi;
        anoi    = anof;
        anof    = tmp;
        tmp     = mesi;
        mesi    = mesf;
        mesf    = tmp;
    } else if ((anoi == anof) && (mesi > mesf)) {
        // Se os anos forem iguais e o mes de inicio for mais "recente"
        // Trocar os meses
        int tmp = mesi;
        mesi    = mesf;
        mesf    = tmp;
    }

    menu_printInfo("encomendas de %d/%d/1 até %d/%d/1", anoi, mesi, anof, mesf);

    // Converter os anos para funcionarem com o o output de 'localtime'
    anoi -= 1900;
    anof -= 1900;
    mesi -= 1;
    mesf -= 1;

    // Criar novos tempos
    struct tm* tm_tF = &(struct tm) {
        .tm_sec   = 0,               // int - seconds after the minute	0-61*
        .tm_min   = 0,               // int - minutes after the hour	0-59
        .tm_hour  = 0,               // int - hours since midnight	0-23
        .tm_mday  = 1,               // int - day of the month	1-31
        .tm_mon   = mesf,            // int - months since January	0-11
        .tm_year  = anof,            // int - years since 1900
        .tm_isdst = tmnow->tm_isdst, // int - Daylight Saving Time flag
    };
    struct tm* tm_tI = &(struct tm) {
        .tm_sec   = 0,               // int - seconds after the minute	0-61*
        .tm_min   = 0,               // int - minutes after the hour	0-59
        .tm_hour  = 0,               // int - hours since midnight	0-23
        .tm_mday  = 1,               // int - day of the month	1-31
        .tm_mon   = mesi,            // int - months since January	0-11
        .tm_year  = anoi,            // int - years since 1900
        .tm_isdst = tmnow->tm_isdst, // int - Daylight Saving Time flag
    };
    // 0 --t1--- ti --t2-- tf --t3-- +infinity
    //      5    10   15   20   25
    // td = difftime(tf, ti) = 10
    // difftime(tf, t1) = 15 (>td)
    // difftime(tf, t2) = 5
    // difftime(tf, t3) = -5 (<0)
    const time_t tf = mktime(tm_tF);
    const time_t ti = mktime(tm_tI);
    const double td = difftime(tf, ti);



    menu_printDiv();
    menu_printHeader("Encomendas Encontradas");
    // Por cada encomenda
    for (size_t i = 0; i < encomendas.size; ++i) {
        // Obter os segundos entre a criação desta encomenda e o tempo final
        const double diff_tempo = difftime(tf, encomendas.data[i].criacao);
        // Imprimir sse: fim >= [esta encomenda] >= inicio
        // td >= diff_tempo >= 0
        if ((td >= diff_tempo) && (diff_tempo >= 0)) {
            printf("   %*lu   |   ", 8, i);
            menu_printEncomendaBrief(&encomendas.data[i], &utilizadores);
            printf("\n");
        }
    }
}

/**
 * Define um par que liga o ID de um utilizador com o preço total que este
 * gastou em encomendas.
 */
typedef struct {
    uint64_t id;
    uint64_t total;
} PAR_ID_TOT;

#define VEC_IMPLEMENTATION
#ifndef pitvec_H
#    define pitvec_H
#    define VEC_TYPE PAR_ID_TOT
#    define VEC_NAME pitvec
#    include "./vector.h"
#endif
#undef VEC_IMPLEMENTATION

/**
 * @brief Lista os utilizadores por preço total gasto em encomendas. Encomendas
 * canceladas não entram no cálculo.
 */
void listagem_Utilizadores_Mais_Gasto() {
    menu_printDiv();
    menu_printHeader("Utilizadores Ordenados Por Dinheiro Gasto");
    // Criar uma lista com todos os utilizadores
    // Par ID do utilizador, total gasto
    pitvec lista = pitvec_new();
    for (size_t u = 0; u < utilizadores.size; ++u) {
        protectFcnCall(pitvec_push(&lista,
                                   (PAR_ID_TOT) {
                                       .total = 0, // Total gasto em encomendas
                                       .id    = u  // ID do utilizador
                                   }),
                       "listagem_Utilizadores_Mais_Gasto - pitvec_push falhou.");
    }

    // Calcular o valor das encomendas feitas por utilizador
    for (size_t e = 0; e < encomendas.size; ++e) {
        const encomenda* encomendaAtual = &encomendas.data[e];
        if (!(encomendas.data[e].tipoEstado & ENCOMENDA_ESTADO_CANCELADA)) {
            lista.data[encomendaAtual->ID_cliente].total += encomenda_CalcPreco(encomendaAtual);
        }
    }

    if (lista.size > 1) {
        // Organizar a lista por total gasto (ordem descendente)
        // l1 maior da lista
        PAR_ID_TOT tmp;
        for (size_t l1 = 0; l1 < lista.size - 1; ++l1) {
            for (size_t l2 = l1 + 1; l2 < lista.size; ++l2) {
                if (lista.data[l1].total < lista.data[l2].total) {
                    tmp            = lista.data[l1];
                    lista.data[l1] = lista.data[l2];
                    lista.data[l2] = tmp;
                }
            }
        }
    }

    // Fazer o display da lista
    menu_printDiv();
    menu_printHeader("Utilizadores Encontrados");
    for (size_t l = 0; l < lista.size; ++l) {
        printf("   %8lu   |   ", lista.data[l].id);
        menu_printUtilizador(utilizadores.data[lista.data[l].id]);
        printf("  Total gasto: %luc\n", lista.data[l].id);
    }

    // Libertar lista
    pitvec_free(&lista);
}

/**
 * @brief Lista encomendas com um certo estado e tipo ordenadas por preço.
 */
void listagem_Encomenda_EmEstado_PorPreco() {
    uint8_t estadoPesquisa = ENCOMENDA_ESTADO_EM_ENTREGA; // Tipo e estado que será pesquisado
    pitvec  lista          = pitvec_new();

    menu_printDiv();
    menu_printHeader("Encomendas Filtradas e Ordenadas");

    // Obter tipo e estado da encomenda do utilizador
    while (1) {
        printf("*** Pesquisa irá encontrar encomendas: ");
        switch (estadoPesquisa & 0xF0) {
            case ENCOMENDA_ESTADO_EM_ENTREGA: printf("Em entrega"); break;
            case ENCOMENDA_ESTADO_EXPEDIDA: printf("Expedidas"); break;
            case ENCOMENDA_ESTADO_ENTREGUE: printf("Entregues"); break;
            case ENCOMENDA_ESTADO_CANCELADA: printf("Canceladas"); break;
            default: printf("Desconhecido"); break;
        }

        printf(" - %s, %s, %s e %s.\n",                                              //
               (estadoPesquisa & ENCOMENDA_TIPO_URGENTE) ? "urgentes" : "regulares", //
               (estadoPesquisa & ENCOMENDA_TIPO_FRAGIL) ? "frageis" : "resistentes", //
               (estadoPesquisa & ENCOMENDA_TIPO_PESADO) ? "pesadas" : "leves",       //
               (estadoPesquisa & ENCOMENDA_TIPO_VOLUMOSO) ? "volumosas" : "pequenas" //
        );

        switch (menu_selection(&(strvec) {.size = 9,
                                          .data = (char*[]) {
                                              "Pesquisar por encomendas de tipo urgente",      // 0
                                              "Pesquisar por encomendas de tipo fragil",       // 1
                                              "Pesquisar por encomendas de tipo pesado",       // 2
                                              "Pesquisar por encomendas de tipo volumoso",     // 3
                                              "Pesquisar por encomendas de estado em entrega", // 4
                                              "Pesquisar por encomendas de estado expedida",   // 5
                                              "Pesquisar por encomendas de estado entregue",   // 6
                                              "Pesquisar por encomendas de estado cancelada",  // 7
                                              "Confirmar Pesquisar"                            // 8
                                          }})) {
            case -1: return;
            case 0:
                estadoPesquisa |=
                    (estadoPesquisa & ENCOMENDA_TIPO_URGENTE) ? ~ENCOMENDA_TIPO_URGENTE : ENCOMENDA_TIPO_URGENTE;
                break;
            case 1:
                estadoPesquisa |=
                    (estadoPesquisa & ENCOMENDA_TIPO_FRAGIL) ? ~ENCOMENDA_TIPO_FRAGIL : ENCOMENDA_TIPO_FRAGIL;
                break;
            case 2:
                estadoPesquisa |=
                    (estadoPesquisa & ENCOMENDA_TIPO_PESADO) ? ~ENCOMENDA_TIPO_PESADO : ENCOMENDA_TIPO_PESADO;
                break;
            case 3:
                estadoPesquisa |=
                    (estadoPesquisa & ENCOMENDA_TIPO_VOLUMOSO) ? ~ENCOMENDA_TIPO_VOLUMOSO : ENCOMENDA_TIPO_VOLUMOSO;
                break;
            case 4: estadoPesquisa = (estadoPesquisa & 0xF) | ENCOMENDA_ESTADO_EM_ENTREGA; break;
            case 5: estadoPesquisa = (estadoPesquisa & 0xF) | ENCOMENDA_ESTADO_EXPEDIDA; break;
            case 6: estadoPesquisa = (estadoPesquisa & 0xF) | ENCOMENDA_ESTADO_ENTREGUE; break;
            case 7: estadoPesquisa = (estadoPesquisa & 0xF) | ENCOMENDA_ESTADO_CANCELADA; break;
            case 8: goto EXIT_LABEL; break; // Sair do loop
        }
    }

EXIT_LABEL:
    printf("*** Pesquisa irá encontrar encomendas: ");
    switch (estadoPesquisa & 0xF0) {
        case ENCOMENDA_ESTADO_EM_ENTREGA: printf("em entrega"); break;
        case ENCOMENDA_ESTADO_EXPEDIDA: printf("expedidas"); break;
        case ENCOMENDA_ESTADO_ENTREGUE: printf("entregues"); break;
        case ENCOMENDA_ESTADO_CANCELADA: printf("canceladas"); break;
        default: printf("desconhecido"); break;
    }

    printf(" - %s, %s, %s e %s.\n",                                              //
           (estadoPesquisa & ENCOMENDA_TIPO_URGENTE) ? "urgentes" : "regulares", //
           (estadoPesquisa & ENCOMENDA_TIPO_FRAGIL) ? "frageis" : "resistentes", //
           (estadoPesquisa & ENCOMENDA_TIPO_PESADO) ? "pesadas" : "leves",       //
           (estadoPesquisa & ENCOMENDA_TIPO_VOLUMOSO) ? "volumosas" : "pequenas" //
    );

    // Criar uma lista com todas as encomendas com 'tipoEstado' compativel.
    protectFcnCall(pitvec_reserve(&lista, encomendas.size),
                   "listagem_Encomenda_EmEstado_PorPreco - pitvec_reserve falhou.");

    for (uint64_t e = 0; e < encomendas.size; ++e) {
        if (encomendas.data[e].tipoEstado == estadoPesquisa) {
            protectFcnCall(pitvec_push(&lista,
                                       (PAR_ID_TOT) {
                                           .id    = e,                                       // ID da encomenda
                                           .total = encomenda_CalcPreco(&encomendas.data[e]) // Preço da encomenda
                                       }),
                           "listagem_Encomenda_EmEstado_PorPreco - pitvec_push falhou.");
        }
    }

    if (lista.size > 1) {
        // Organizar lista por preço (descendente)
        // l1 maior da lista
        PAR_ID_TOT tmp;
        for (size_t l1 = 0; l1 < lista.size - 1; ++l1) {
            for (size_t l2 = l1 + 1; l2 < lista.size; ++l2) {
                if (lista.data[l1].total < lista.data[l2].total) {
                    tmp            = lista.data[l1];
                    lista.data[l1] = lista.data[l2];
                    lista.data[l2] = tmp;
                }
            }
        }
    }

    // Fazer o display das encomendas
    menu_printDiv();
    menu_printHeader("Encomendas Encontradas");
    for (size_t l = 0; l < lista.size; ++l) {
        printf("   %8lu   |   ", lista.data[l].id);
        menu_printEncomendaBrief(&encomendas.data[lista.data[l].id], &utilizadores);
        printf("  Preço: %luc\n", lista.data[l].total);
    }

    // Libertar lista
    pitvec_free(&lista);
}

/**
 * @def SEGUNDOS_EM_7_DIAS
 *              O número de segundos numa semana.
 */
#define SEGUNDOS_EM_7_DIAS (7 * 24 * 60 * 60)

/**
 * @brief Imprime encomendas realizadas nos últimos 7 dias.
 */
void listagem_Artigos_Semana() {
    menu_printDiv();
    menu_printHeader("Encomendas Nos Ultimos Sete Dias");

    time_t now = time(NULL);
    for (size_t e = 0; e < encomendas.size; ++e) {
        const double dt = difftime(now, encomendas.data[e].criacao);
        if ((dt > 0) && (dt < SEGUNDOS_EM_7_DIAS)) {
            printf("   %8lu   |   ", e);
            menu_printEncomendaBrief(&encomendas.data[e], &utilizadores);
            printf("   Artigos:\n");
            for (size_t a = 0; a < artigos.size; ++a) {
                printf("             %8lu   |   ", a);
                menu_printArtigo(&artigos.data[a]);
                printf("\n");
            }
        }
    }
}

/**
 * @brief Imprime o recibo para uma única encomenda e não para as encomendas
 * realizadas no mês.
 */
void listagem_imprimir_recibo() {
    menu_printDiv();
    menu_printHeader("Selecionar Encomenda Para Imprimir");
    int op  = -2;
    int max = encomendas.size - 1;
    while (op == -2) {
        printf("   Opção      |   Item\n");
        printf("         -2   |   Reimprimir\n");
        printf("         -1   |   Sair\n");
        funcional_consultar_estados_encomendas();
        menu_readIntMinMax(-2, max, &op);
        menu_printDiv();
    }
    if (op == -1) return;
    // Verificar se o utilizador atual é um diretor ou se a encomenda que
    // foi selecionada foi formalizada pelo utilizador atual.
    if ((utilizadores.data[utilizadorAtual].tipo != UTILIZADOR_DIRETOR) &&
        (utilizadorAtual != encomendas.data[op].ID_cliente)) {
        menu_printError("não tem premissões para imprimir este recibo!");
    } else
        menu_printEncomendaDetail(&encomendas.data[op], &utilizadores);
}

/**
 * @brief Premite aceder as listagens extra, e escolher qual listagem utilizar.
 */
void interface_outrasListagens() {
    while (1) {
        menu_printDiv();
        switch (menu_selection(&(strvec) {.size = 5,
                                          .data = (char*[]) {
                                              "Listagem - Recibo de Encomenda",                        // 0
                                              "Listagem - Encomendas num certo periodo de tempo",      // 1
                                              "Listagem - Utilizadores que mais gastaram",             // 2
                                              "Listagem - Encomendas filtradas e ordenadas por preço", // 3
                                              "Listagem - Artigos propostos na ultima semana"          // 4
                                          }})) {
            case -1: return;
            case 0: listagem_imprimir_recibo(); break;
            case 1: listagem_Encomendas_Periodo_de_Tempo(); break;
            case 2: listagem_Utilizadores_Mais_Gasto(); break;
            case 3: listagem_Encomenda_EmEstado_PorPreco(); break;
            case 4: listagem_Artigos_Semana(); break;
        }
    }
}