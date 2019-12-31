#include "outrasListagens.h"

#include "menu.h"
#include "utilities.h"

void listagem_Encomendas_Periodo_de_Tempo() {
    menu_printDiv();
    menu_printHeader("Listagem de Encomendas em Espaço de Tempo");

    time_t now = time(NULL);
    struct tm* tmnow = localtime(&now);

    int mesi = 0;
    int mesf = 0;
    int anoi = 0;
    int anof = 0;

    printf("Introduzir mês de inicio para a pesquisa $");
    menu_readIntMinMax(1, 12, &mesi);

    printf("Introduzir ano de inicio para a pesquisa $");
    menu_readIntMinMax(2000, tmnow->tm_year + 1970, &anoi);

    printf("Introduzir mês de fim para a pesquisa $");
    menu_readIntMinMax(1, 12, &mesf);

    printf("Introduzir ano de fim para a pesquisa $");
    menu_readIntMinMax(2000, tmnow->tm_year + 1970, &anof);

    // 0 --- inicio --- fim --- +infinito
    // inicio < fim
    if(anoi > anof) {
        int tmp = anoi;
        anoi = anof;
        anof = tmp;
        tmp = mesi;
        mesi = mesf;
        mesf = tmp;
    } else if ((anoi == anof) && (mesi > mesf)) {
        int tmp = mesi;
        mesi = mesf;
        mesf = tmp;
    }

    anoi -= 1970;
    anof -= 1970;
    mesi -= 1;
    mesf -= 1;

    menu_printDiv();
    menu_printHeader("Encomendas Encontradas");
    for (size_t i = 0; i < encomendas.size; ++i) {
        struct tm* const t = localtime(&(encomendas.data[i].criacao));
        const int ano = t->tm_year;
        const int mes = t->tm_mon;
        // imprimir sse inicio <= X <= mes
        if (
            ( anoi < ano && ano < anof ) ||
            (( anoi == ano) && (mesi <= mes)) ||
            (( anof == ano) && (mes <= mesf))
        ) {
            printf("   %*lu   |   ", 8, i);
            menu_printEncomendaBrief(&(encomendas.data[i]), &utilizadores);
            printf("\n");
        }
    }
}

typedef struct {
    uint64_t ID_utilizador;
    uint64_t totalGasto;
} par_idc_tG;

#define VEC_IMPLEMENTATION
#ifndef pidctgvec_H
#define pidctgvec_H
#define VEC_TYPE         par_idc_tG
#define VEC_NAME         pidctgvec
#define VEC_DEALOC(X)
#include "./vector.h"
#endif
#undef VEC_IMPLEMENTATION

void listagem_Utilizadores_Mais_Gasto() {
    menu_printDiv();
    menu_printHeader("Utilizadores Ordenados Por Dinheiro Gasto");
    // Criar uma lista com todos os utilizadores
    pidctgvec lista = pidctgvec_new();
    for(size_t u = 0; u < utilizadores.size; ++u) {
        protectFcnCall(
            pidctgvec_push(&lista, (par_idc_tG) {
                .totalGasto = 0,
                .ID_utilizador = u
            } ),
            "listagem_Utilizadores_Mais_Gasto - pidctgvec_push falhou.");
    }

    // Calcular o valor das encomendas feitas por utilizador
    for(size_t e = 0; e < encomendas.size; ++ e) {
        if(!(encomendas.data[e].tipoEstado & ENCOMENDA_ESTADO_CANCELADA)) {
            lista.data[encomendas.data[e].ID_cliente].totalGasto += encomenda_CalcPreco(&(encomendas.data[e]));
        }
    }

    // l1 maior da lista
    par_idc_tG tmp;
    for(size_t l1 = 0; l1 < lista.size - 1; ++l1) {
        for(size_t l2 = l1 + 1; l2 < lista.size; ++l2) {
            if (lista.data[l1].totalGasto < lista.data[l2].totalGasto) {
                tmp = lista.data[l1];
                lista.data[l1] = lista.data[l2];
                lista.data[l2] = tmp;
            }
        }
    }

    menu_printDiv();
    menu_printHeader("Utilizadores Encontrados");
    for(size_t l = 0; l < lista.size; ++l) {
        printf("   %8lu   |   ", lista.data[l].ID_utilizador);
        menu_printUtilizador(utilizadores.data[lista.data[l].ID_utilizador]);
        printf("  Total gasto: %luc\n", lista.data[l].totalGasto);
    }

    pidctgvec_free(&lista);
}

typedef struct {
    uint64_t ID_encomenda;
    uint64_t preco;
} par_ide_p;

#define VEC_IMPLEMENTATION
#ifndef pidepvec_H
#define pidepvec_H
#define VEC_TYPE         par_ide_p
#define VEC_NAME         pidepvec
#define VEC_DEALOC(X)
#include "./vector.h"
#endif
#undef VEC_IMPLEMENTATION

void listagem_Encomenda_EmEstado_PorPreco() {
    menu_printDiv();
    menu_printHeader("Encomendas Filtradas e Ordenadas");
    uint8_t estadoPesquisa = 0;
    pidepvec lista = pidepvec_new();

    while (1){
        printf("*** Pesquisa irá encontrar encomendas: ");
        switch (estadoPesquisa&0xF0) {
            case ENCOMENDA_ESTADO_EM_ENTREGA: printf("em entrega"); break;
            case ENCOMENDA_ESTADO_EXPEDIDA  : printf("expedidas");  break;
            case ENCOMENDA_ESTADO_ENTREGUE  : printf("entregues");  break;
            case ENCOMENDA_ESTADO_CANCELADA : printf("canceladas"); break;
            default: printf("desconhecido"); break;
        }

        printf(" - %s, %s, %s e %s.\n",
            (estadoPesquisa & ENCOMENDA_TIPO_URGENTE ) ? "urgentes"  : "regulares",
            (estadoPesquisa & ENCOMENDA_TIPO_FRAGIL  ) ? "frageis"   : "resistentes",
            (estadoPesquisa & ENCOMENDA_TIPO_PESADO  ) ? "pesadas"   : "leves",
            (estadoPesquisa & ENCOMENDA_TIPO_VOLUMOSO) ? "volumosas" : "pequenas"
        );

        switch (menu_selection(& (strvec) {
            .size = 9,
            .data = (char*[]) {
                "Pesquisar por encomendas de tipo urgente",
                "Pesquisar por encomendas de tipo fragil",
                "Pesquisar por encomendas de tipo pesado",
                "Pesquisar por encomendas de tipo volumoso",
                "Pesquisar por encomendas de estado em entrega",
                "Pesquisar por encomendas de estado expedida",
                "Pesquisar por encomendas de estado entregue",
                "Pesquisar por encomendas de estado cancelada",
                "Confirmar Pesquisar"
            }
        })) {
            case -1: return;
            case  0:
                estadoPesquisa |= (estadoPesquisa & ENCOMENDA_TIPO_URGENTE ) ? ~ENCOMENDA_TIPO_URGENTE  : ENCOMENDA_TIPO_URGENTE ;
            break;
            case  1:
                estadoPesquisa |= (estadoPesquisa & ENCOMENDA_TIPO_FRAGIL  ) ? ~ENCOMENDA_TIPO_FRAGIL   : ENCOMENDA_TIPO_FRAGIL  ;
            break;
            case  2:
                estadoPesquisa |= (estadoPesquisa & ENCOMENDA_TIPO_PESADO  ) ? ~ENCOMENDA_TIPO_PESADO   : ENCOMENDA_TIPO_PESADO  ;
            break;
            case  3:
                estadoPesquisa |= (estadoPesquisa & ENCOMENDA_TIPO_VOLUMOSO) ? ~ENCOMENDA_TIPO_VOLUMOSO : ENCOMENDA_TIPO_VOLUMOSO;
            break;
            case  4: estadoPesquisa = (estadoPesquisa & 0xF) | ENCOMENDA_ESTADO_EM_ENTREGA; break;
            case  5: estadoPesquisa = (estadoPesquisa & 0xF) | ENCOMENDA_ESTADO_EXPEDIDA; break;
            case  6: estadoPesquisa = (estadoPesquisa & 0xF) | ENCOMENDA_ESTADO_ENTREGUE; break;
            case  7: estadoPesquisa = (estadoPesquisa & 0xF) | ENCOMENDA_ESTADO_CANCELADA; break;
            case  8: goto EXIT_LABEL; break;
        }
    }

    EXIT_LABEL:
    protectFcnCall(pidepvec_reserve(&lista, encomendas.size), "listagem_Encomenda_EmEstado_PorPreco - pidepvec_reserve falhou.")

    for(uint64_t e = 0; e < encomendas.size; ++ e) {
        if(encomendas.data[e].tipoEstado == estadoPesquisa) {
            protectFcnCall(
                pidepvec_push(&lista, (par_ide_p) {
                    .ID_encomenda = e,
                    .preco = encomenda_CalcPreco(&(encomendas.data[e]))
                }),
                "listagem_Encomenda_EmEstado_PorPreco - pidepvec_push falhou.");
        }
    }

    // l1 maior da lista
    par_ide_p tmp;
    for(size_t l1 = 0; l1 < lista.size - 1; ++l1) {
        for(size_t l2 = l1 + 1; l2 < lista.size; ++l2) {
            if (lista.data[l1].preco < lista.data[l2].ID_encomenda) {
                tmp = lista.data[l1];
                lista.data[l1] = lista.data[l2];
                lista.data[l2] = tmp;
            }
        }
    }

    menu_printDiv();
    menu_printHeader("Encomendas Encontradas");
    for(size_t l = 0; l < lista.size; ++l) {
        printf("   %8lu   |   ", lista.data[l].ID_encomenda);
        menu_printEncomendaBrief(&(encomendas.data[lista.data[l].ID_encomenda]), &utilizadores);
        printf("  Preço: %luc\n", lista.data[l].preco);
    }

    pidepvec_free(&lista);
}

#define SEGUNDOS_EM_7_DIAS 7 * 24 * 60 * 60

void listagem_Artigos_Semana() {
    menu_printDiv();
    menu_printHeader("Encomendas Nos Ultimos Sete Dias");
    time_t now = time(NULL);
    for (size_t e = 0; e < encomendas.size; ++e) {
        if (difftime(now, encomendas.data[e].criacao) <= SEGUNDOS_EM_7_DIAS) {
            printf("   %8lu   |   ", e);
            menu_printEncomendaBrief(&(encomendas.data[e]), &utilizadores);
            printf("   Artigos:\n");
            for (size_t a = 0; a < artigos.size; ++a) {
                printf("             %8lu   |   ", a);
                menu_printArtigo(&(artigos.data[a]));
                printf("\n");
            }
        }
    }
}

void listagem_imprimir_recibo() {
    menu_printHeader("Selecionar Encomenda Para Imprimir");
    int op = -2;
    int max = encomendas.size-1;
    while(op == -2) {
        printf("   Opção      |   Item\n");
        printf("         -2   |   Reimprimir\n");
        printf("         -1   |   Sair\n");
        funcional_consultar_estados_encomendas();
        while (!menu_readIntMinMax(-2, max, &op));
        menu_printDiv();
    }
    if(op == -1) return;
    if((utilizadores.data[utilizadorAtual].tipo != UTILIZADOR_DIRETOR) && (utilizadorAtual != encomendas.data[op].ID_cliente) ) {
        menu_printError("não tem premissões para imprimir este recibo!");
    } else menu_printEncomendaDetail(&(encomendas.data[op]), &utilizadores);
}

void interface_outrasListagens () {
    while(1) {
        switch (menu_selection(& (strvec) {
            .size = 5,
            .data = (char*[]) {
                "Listagem - Recibo de Encomenda",
                "Listagem - Encomendas num certo periodo de tempo",
                "Listagem - Utilizadores que mais gastaram",
                "Listagem - Encomendas filtradas e ordenadas por preço",
                "Listagem - Artigos propostos na ultima semana"
            }
        })) {
            case -1: return;
            case  0: listagem_imprimir_recibo();             break;
            case  1: listagem_Encomendas_Periodo_de_Tempo(); break;
            case  2: listagem_Utilizadores_Mais_Gasto();     break;
            case  3: listagem_Encomenda_EmEstado_PorPreco(); break;
            case  4: listagem_Artigos_Semana();              break;
        }
    }
}