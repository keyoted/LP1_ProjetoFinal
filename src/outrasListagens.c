#include "outrasListagens.h"

void listagem_Encomendas_Periodo_de_Tempo() {
    menu_printDiv();
    menu_printHeader("Listagem de Encomendas");

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
            menu_printEncomendaBrief(&(encomendas.data[i]));
            printf("\n");
        }
    }
}

typedef struct {
    uint8_t  NIF_Clinete[9];
    char*    nome;
    uint64_t totalGasto;
    size_t   ID;
} par_utilizadorID_totalGasto;

#define VEC_IMPLEMENTATION
#ifndef puitgvec_H
#define puitgvec_H
#define VEC_TYPE         par_utilizadorID_totalGasto
#define VEC_NAME         puitgvec
#define VEC_DEALOC(X)
#include "./vector.h"
#endif
#undef VEC_IMPLEMENTATION

void listagem_Utilizadores_Mais_Gasto() {
    puitgvec lista = puitgvec_new();
    for(size_t u = 0; u < utilizadores.size; ++u) {
        puitgvec_push(&lista, (par_utilizadorID_totalGasto) {
            .totalGasto = 0,
            .ID = u,
            .nome = utilizadores.data[u].nome
        } );
        memcpy(lista.data[u].NIF_Clinete, utilizadores.data[u].NIF, 9);
    }

    for(size_t e = 0; e < encomendas.size; ++ e) {
        for(size_t l = 0; l < lista.size; ++l) {
            if(memcmp(lista.data[l].NIF_Clinete, encomendas.data[e].NIF_cliente, 9) == 0) {
                lista.data[l].totalGasto += encomenda_CalcPreco(&(encomendas.data[e]));
                break;
            }
        }
    }

    // l1 maior da lista
    par_utilizadorID_totalGasto tmp;
    for(size_t l1 = 0; l1 < lista.size - 1; ++l1) {
        for(size_t l2 = l1 + 1; l2 < lista.size; ++l2) {
            if (lista.data[l1].totalGasto > lista.data[l2].totalGasto) {
                tmp = lista.data[l1];
                lista.data[l1] = lista.data[l2];
                lista.data[l2] = tmp;
            }
        }
    }

    menu_printDiv();
    menu_printHeader("Utilizadores Encontrados");
    for(size_t l = 0; l < lista.size; ++l) {
        printf("   %*lu   |   %s (%.9s)  Total gasto: %luc\n", 8, lista.data[l].ID, lista.data[l].nome, lista.data[l].NIF_Clinete, lista.data[l].totalGasto);
    }

    free(lista.data);
}

void listagem_Encomenda_EmEstado_PorPreco() {
}

void listagem_Artigos_Semana() {
}

void interface_outrasListagens () {
}