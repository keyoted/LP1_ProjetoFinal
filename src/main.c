// TODO: apagar, apenas util para debug
#include <stdlib.h>
#include <sys/stat.h>

#define  VEC_IMPLEMENTATION
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <time.h>

#define UTILIZADOR_INVALIDO ~((size_t)0)
#include "utilities.h"
#include "menu.h"
#include "artigo.h"
#include "encomenda.h"
#include "utilizador.h"
#include "outrasListagens.h"

#ifndef artigovec_H
#define artigovec_H
#define VEC_TYPE         artigo
#define VEC_NAME         artigovec
#define VEC_DEALOC(X)    freeArtigo(&X)
#include "./vector.h"
#endif

#ifndef encomendavec_H
#define encomendavec_H
#define VEC_TYPE         encomenda
#define VEC_NAME         encomendavec
#define VEC_DEALOC(X)    freeEncomenda(&X)
#include "./vector.h"
#endif

#ifndef utilizadorvec_H
#define utilizadorvec_H
#define VEC_TYPE         utilizador
#define VEC_NAME         utilizadorvec
#define VEC_DEALOC(X)    freeUtilizador(&X)
#include "./vector.h"
#endif

artigovec      artigos;                                 // Artigos da seção atual, que ainda não foram formalizados numa encomenda
encomendavec   encomendas;                              // Encomendas formalizadas
utilizadorvec  utilizadores;                            // Utilizadores existentes no registo, index 0 é diretor
size_t         utilizadorAtual = UTILIZADOR_INVALIDO;   // Index do utilizador atual
precos_tt_cent tabelaPrecos;                            // Preço em centimos por cada tipo de transporte

int vecPrintUserPredicate (utilizador item, int* userdata) {
    printf("   %*d   |   ", 8, ++(*userdata));
    menu_printUtilizador(item);
    printf("\n");
    return 0;
}

void funcional_recibo_mensal() {
    int mes;
    int ano;
    printf("Inserir ano do recibo (0 para o atual) $ ");
    menu_readInt(&ano);
    printf("Inserir mês do recibo (0 para o atual) $ ");
    menu_readInt(&mes);

    time_t timeNow = time(NULL);
    if(ano == 0) {
        ano = localtime(&timeNow)->tm_year;
    } else ano -= 1900;
    if(mes == 0) {
        mes = localtime(&timeNow)->tm_mon;
    } else mes -= 1;

    if(utilizadores.data[utilizadorAtual].tipo != UTILIZADOR_DIRETOR) {
        menu_printReciboMensal(utilizadores.data[utilizadorAtual].NIF, mes, ano, &encomendas);
    } else {
        menu_printHeader("Selecionar Utilizador Para Quem Imprimir o Recibo");
        int op = -2;
        int max = utilizadores.size-1;
        while(op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            int i = -1;
            utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_predicate_t)&vecPrintUserPredicate, (void*)&i);
            while (!menu_readIntMinMax(-2, max, &op));
            menu_printDiv();
        }
        if(op == -1) return;
        menu_printReciboMensal(utilizadores.data[op].NIF, mes, ano, &encomendas);
    }
}

void funcional_consultar_estados_encomendas() {
    if(utilizadores.data[utilizadorAtual].tipo == UTILIZADOR_DIRETOR) {
        for(size_t i = 0; i < encomendas.size; ++i) {
            printf("   %*lu   |   ", 8, i);
            menu_printEncomendaBrief(&(encomendas.data[i]));
            printf("\n");
        }
    } else {
        for(size_t i = 0; i < encomendas.size; ++i) {
            if( memcmp(utilizadores.data[utilizadorAtual].NIF, encomendas.data[i].NIF_cliente, 9) == 0 ) {
                printf("   %*lu   |   ", 8, i);
                menu_printEncomendaBrief(&(encomendas.data[i]));
                printf("\n");
            }
        }
    }
}

void interface_imprimir_recibo() {
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
    if((utilizadores.data[utilizadorAtual].tipo != UTILIZADOR_DIRETOR) && (memcmp(utilizadores.data[utilizadorAtual].NIF, encomendas.data[op].NIF_cliente, 9) != 0) ) {
        menu_printError("não tem premissões para imprimir este recibo!");
    } else menu_printEncomendaDetail(&(encomendas.data[op]));
}

void interface_editar_morada(morada* const m, int isNew) {

    if(isNew) {
        printf("Introduzir Morada $ ");
        *m = newMorada();
    } else printf("Introduzir Morada (%s) $ ", m->morada);

    freeCHK(m->morada, menu_readString(stdin));

    char* stmp = NULL;
    while (1) {
        if(isNew) printf("Introduzir Código Postal (XXXX-XXX) ");
        else      printf("Introduzir Código Postal (XXXX-XXX) (%.4s-%.3s) ", m->codigoPostal, &(m->codigoPostal[4]));

        freeN(stmp);
        while(!stmp) { printf("$ "); stmp = menu_readString(stdin); }

        if (strlen(stmp) != 8) {
            menu_printError("tem que introduzir 8 characteres, %d introduzidos.", strlen(stmp));
        } else {
            m->codigoPostal[0] = stmp[0];
            m->codigoPostal[1] = stmp[1];
            m->codigoPostal[2] = stmp[2];
            m->codigoPostal[3] = stmp[3];
            m->codigoPostal[4] = stmp[5];
            m->codigoPostal[5] = stmp[6];
            m->codigoPostal[6] = stmp[7];
            if (!morada_eCPValido(m->codigoPostal)) {
                menu_printError("Código Postal introduzido é inválido.");
            } else break;
        }
    }
    free(stmp);
}

int utilizadorAtivadoNIFCompareVecPredicate(utilizador element, uint8_t compare[9]) {
    if(element.tipo == UTILIZADOR_DESATIVADO) return 0;
    return memcmp(element.NIF, compare, 9) == 0;
}

int utilizadorNIFCompareVecPredicate(utilizador element, uint8_t compare[9]) {
    return memcmp(element.NIF, compare, 9) == 0;
}

void funcional_editar_diretor(size_t op) {
    menu_printDiv();
    menu_printHeader("Editar Diretor");

    freeUtilizador(&(utilizadores.data[op]));
    utilizadores.data[op] = newUtilizador();
    utilizador* diretor = & utilizadores.data[op];
    diretor->tipo = UTILIZADOR_DIRETOR;

    printf("Introduzir Nome $ ");
    freeCHK(diretor->nome, menu_readString(stdin));

    char* stmp = NULL;
    while (1) {
        printf("Introduzir NIF ");
        freeN(stmp);
        while(!stmp) { printf("$ "); stmp = menu_readString(stdin); }
        if (strlen(stmp) != 9) {
            menu_printError("tem que introduzir 9 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eNIFValido((uint8_t*)stmp)) {
            menu_printError("NIF introduzido é inválido.");
        } else {
            memcpy(&diretor->NIF, stmp, sizeof(uint8_t)*9);
            break;
        }
    }
    free(stmp);
    menu_printInfo("diretor editado com sucesso!");
}

void funcional_editar_utilizador(size_t index) {
    menu_printDiv();
    menu_printHeader("Edição de Utilizador");
    utilizador* u = &(utilizadores.data[index]);

    printf("Introduzir Nome (%s) $ ", u->nome);
    freeCHK(u->nome, menu_readString(stdin));

    interface_editar_morada(&(u->endereco), 0);

    char* stmp = NULL;
    while (1) {
        printf("Introduzir numero do cartao de cidadão (%.12s) ", u->CC);
        freeN(stmp);
        while(!stmp) { printf("$ "); stmp = menu_readString(stdin); }
        if (strlen(stmp) != 12) {
            menu_printError("tem que introduzir 12 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eCCValido((uint8_t*)stmp)) {
            menu_printError("CC introduzido é inválido.");
        } else {
            memcpy(&u->CC, stmp, sizeof(uint8_t)*12);
            break;
        }
    }
    free(stmp);

    menu_printInfo("utilizador %s (%.9s) editado com sucesso!", u->nome, u->NIF);
}

void funcional_consultar_tabela_de_precos(int printDist, int printTable) {
    if (printDist) {
        menu_printHeader("Tabela de Distancia");
        printf("ORIGEM DESTINO  0           1           2           3           4           5           6           7           8           9\n");
        for (int origem = 0; origem < 10; ++origem) {
            printf("  %d  |        ", origem);
            for (int destino = 0; destino < 10; ++destino) {
                printf("  %7.4f  |", (double)tabelaPrecos.MULT_CP[origem][destino]);
            }
            printf("\n");
        }
    }
    if (printTable) {
        menu_printHeader("Tabela de Preços Base");
        printf("Preço regular:  %luc\n", tabelaPrecos.REGULAR );
        printf("Preço urgente:  %luc\n", tabelaPrecos.URGENTE );
        printf("Preço volumoso: %luc\n", tabelaPrecos.VOLUMOSO);
        printf("Preço fragil:   %luc\n", tabelaPrecos.FRAGIL  );
        printf("Preço pesado:   %luc\n", tabelaPrecos.PESADO  );
        printf("Preço por Km:   %luc\n", tabelaPrecos.POR_KM  );
    }
}

void interface_alterar_tabela_precos(){
    funcional_consultar_tabela_de_precos(0, 1);
    menu_printDiv();
    menu_printHeader("Editar preços");
    menu_printInfo("selecione o que editar");
    int edit = menu_selection(&(strvec){
        .data = (char*[]){
            "Editar preço regular",
            "Editar preço urgente",
            "Editar preço volumoso",
            "Editar preço fragil",
            "Editar preço pesado",
            "Editar preço por Km",
        },
        .size = 6
    });
    if(edit == -1) return;

    int novoPreco;
    while (1) {
        printf("Introduza novo preço (em centimos) $ ");
        menu_readInt(&novoPreco);
        if(novoPreco < 0) {
            menu_printError("Preço não pode ser negativo");
        } else break;
    }

    switch (edit) {
        case 0: tabelaPrecos.REGULAR  = novoPreco; return;
        case 1: tabelaPrecos.URGENTE  = novoPreco; return;
        case 2: tabelaPrecos.VOLUMOSO = novoPreco; return;
        case 3: tabelaPrecos.FRAGIL   = novoPreco; return;
        case 4: tabelaPrecos.PESADO   = novoPreco; return;
        case 5: tabelaPrecos.POR_KM   = novoPreco; return;
    }
}

void interface_alterar_tabela_distancias(){
    funcional_consultar_tabela_de_precos(1, 0);
    menu_printInfo("selecione modo de edição");
    int modo = menu_selection(&(strvec){
        .data = (char*[]){
            "Editar preço de elemento",
            "Editar preço de linha (todos os destinos de uma origem)",
        },
        .size = 2
    });
    if(modo == -1) return;

    int origem;
    int destino;
    _Float32 novoVal;
    menu_printInfo("selecione linha de origem");
    menu_readIntMinMax(0,9, &origem);

    if(modo == 0) {
        menu_printInfo("selecione coluna de destino");
        menu_readIntMinMax(0,9, &destino);
        while (1) {
            printf("Introduza novo valor decimal para { origem:%d, destino:%d } $ ", origem, destino);
            if (menu_read_Float32(&novoVal)) {
                if (novoVal < 0) {
                    menu_printError("valor tem que ser maior que 0.");
                } else {
                    tabelaPrecos.MULT_CP[origem][destino] = novoVal;
                    break;
                }
            }
        }
    } else {
        for(destino = 0; destino < 10; ++destino) {
            while (1) {
                printf("Introduza novo valor decimal para { origem:%d, destino:%d } $ ", origem, destino);
                if (menu_read_Float32(&novoVal)) {
                    if (novoVal < 0) {
                        menu_printError("valor tem que ser maior que 0.");
                    } else {
                        tabelaPrecos.MULT_CP[origem][destino] = novoVal;
                        break;
                    }
                }
            }
        }
    }
}

void interface_alterar_utilizadores(){
    while (1) {
        menu_printDiv();
        menu_printHeader("Selecionar Utilizador Para Editar");
        int op = -2;
        int max = utilizadores.size-1;
        while(op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            int i = -1;
            utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_predicate_t)&vecPrintUserPredicate, (void*)&i);
            while (!menu_readIntMinMax(-2, max, &op));
            menu_printDiv();
        }
        if(op == -1) return;

        menu_printDiv();
        menu_printHeader("Selecionar operação");
        printf("Utilizador selecionado:");
        menu_printUtilizador(utilizadores.data[op]);
        printf("\n");
        switch( menu_selection( &(strvec) {
            .data = (char*[]) {
                "Marcar como cliente",
                "Marcar como desativado",
                "Marcar como diretor"
                "Editar utilizador" } ,
            .size = 4
        })) {
            case -1: return;
            case  0:
                if(op==0) {
                    menu_printError("Utilizador 0 é sempre diretor");
                } else utilizadores.data[op].tipo = UTILIZADOR_CLIENTE;
            break;
            case 1:
                if(utilizadores.data[utilizadorAtual].tipo == UTILIZADOR_DIRETOR) {
                    menu_printError("Um Diretor está sempre ativo");
                } else utilizadores.data[op].tipo = UTILIZADOR_DESATIVADO;
            break;
            case  2:
                utilizadores.data[op].tipo = UTILIZADOR_DIRETOR;
            break;
            case  3:
                switch (utilizadores.data[utilizadorAtual].tipo) {
                    case UTILIZADOR_CLIENTE:
                        funcional_editar_utilizador(op);
                    break;
                    case UTILIZADOR_DESATIVADO:
                        menu_printInfo("está a editar um utilizador desativado");
                        funcional_editar_utilizador(op);
                    break;
                    case UTILIZADOR_DIRETOR:
                        funcional_editar_diretor(op);
                    break;
                }
            break;
        }
    }
}

int vecPrintEncomendaPredicate (encomenda item, int* userdata) {
    printf("   %*d   |   ", 8, ++(*userdata));
    menu_printEncomendaBrief(&(item));
    printf("\n");
    return 0;
}

void interface_editar_estados_encomendas(){
    while (1) {
        menu_printDiv();
        menu_printHeader("Selecionar Encomenda Para Editar");
        int op = -2;
        int max = encomendas.size-1;
        while(op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            int i = -1;
            encomendavec_iterateFW(&encomendas, (encomendavec_predicate_t)&vecPrintEncomendaPredicate, (void*)&i);
            while (!menu_readIntMinMax(-2, max, &op));
            menu_printDiv();
        }
        if(op == -1) return;

        encomenda* e = &(encomendas.data[op]);
        menu_printDiv();
        while (1) {
            menu_printHeader("Selecionar operação");
            switch (menu_selection(&(strvec){
                .data = (char*[]){
                    "Marcar encomenda como expedida",
                    "Marcar encomenda como cancelada",
                    "Marcar encomenda como em entrega",
                    "Marcar encomenda como entregue"
                },
                .size = 4
            })) {
                case -1: return;
                case  0: encomenda_ESTADO_EXPEDIDA(e);   break;
                case  1: encomenda_ESTADO_CANCELADA(e);  break;
                case  2: encomenda_ESTADO_EM_ENTREGA(e); break;
                case  3: encomenda_ESTADO_ENTREGUE(e);   break;
            }
        }
    }
}

void interface_diretor() {
    while (1) {
        menu_printDiv();
        menu_printHeader("Menu de Diretor");
        switch (menu_selection(&(strvec){
            .data = (char*[]){
                "Editar dados de acesso",
                "Alterar tabela de preços",
                "Alterar tabela de distancias",
                "Alterar estado dos utilizadores",
                "Alterar estados das encomendas",
                "Imprimir recibo de encomenda",
                "Imprimir recibo mensal de um utilizador",
                "Consultar tabela de preços",
                "Outras listagens"
            },
            .size = 9
        })) {
            case -1: return;
            case  0: funcional_editar_diretor(utilizadorAtual); break;
            case  1: interface_alterar_tabela_precos();         break;
            case  2: interface_alterar_tabela_distancias();     break;
            case  3: interface_alterar_utilizadores();          break;
            case  4: interface_editar_estados_encomendas();     break;
            case  5: interface_imprimir_recibo();               break;
            case  6: funcional_recibo_mensal();                 break;
            case  7: funcional_consultar_tabela_de_precos(1,1); break;
            case  8: interface_outrasListagens();               break;
        }
    }
}

void funcional_desativar_perfil(){
    utilizadores.data[utilizadorAtual].tipo = UTILIZADOR_DESATIVADO;
    utilizadorAtual = ~0;
    menu_printInfo("utilizador desativado com sucesso!");
}

// Artigos size is assumed to be diffrent than 0
int funcional_formalizar_encomenda(artigovec* artigos, encomenda* e) {
    menu_printDiv();
    menu_printHeader("Formalizar Encomenda");

    menu_printInfo("utilizar a sua morada como morada de entrega?");
    morada dest;
    switch (menu_selection(&(strvec){
        .data = (char*[]){
            "Utilizar a minha morada como morada de entrega",
            "Definir outra morada como morada de entrega"
        },
        .size = 2
    })) {
        case -1: return 0;
        case  0: dest = morada_dup(utilizadores.data[utilizadorAtual].endereco); break;
        case  1: interface_editar_morada(&dest, 1); break;
    }

    menu_printInfo("introduzir morada de origem.");
    morada org;
    interface_editar_morada(&org, 1);

    int dist;
    while (1) {
        printf("Introduzir Distancia (em Km) $ ");
        menu_readInt(&dist);
        if(dist < 0) menu_printError("distancia negativa não é possivél.");
        else break;
    }
    *e = encomenda_formalizar(*artigos, tabelaPrecos, utilizadores.data[utilizadorAtual].NIF, org, dest, dist);
    *artigos = artigovec_new();
    while (1) {
        menu_printInfo("definir encomenda como urgente e/ou fragil?");
        switch (menu_selection(&(strvec){
            .data = (char*[]){
                "Definir encomenda como urgente",
                "Definir encomenda como fragil",
                "Continuar"
            },
            .size = 3
        })) {
            case -1: goto END_OF_LOOP;
            case  0: encomenda_TIPO_URGENTE(e);      break;
            case  1: encomenda_TIPO_FRAGIL_togle(e); break;
            case  2: goto END_OF_LOOP;
        }
    }
    END_OF_LOOP:
    menu_printInfo("encomenda formalizada com sucesso!");
    return 1;
}

int vecPrintArtigoPredicate (artigo item, int* userdata) {
    printf("   %*d   |   ", 8, ++(*userdata));
    menu_printArtigo(&(item));
    printf("\n");
    return 0;
}

int funcional_editar_artigo (artigo* a, int isDeletable) {
    if(isDeletable) {
        switch (menu_selection(&(strvec){
            .data = (char*[]){
                "Apagar artigo",
                "Editar Artigo",
            },
            .size = 2
        })) {
            case -1: return 1;
            case  0: freeArtigo(a); return 0;
            case  1: break;
        }
    }

    printf("Introduzir Nome do Artigo (%s) $ ", a->nome);
    freeCHK(a->nome, menu_readString(stdin));

    if(a->tratamentoEspecial) {
        printf("Introduzir Tratamento especial para o artigo Artigo (%s) $ ", a->tratamentoEspecial);
        freeN(a->tratamentoEspecial);
    } else printf("Introduzir Tratamento especial para o artigo Artigo $ ");
    a->tratamentoEspecial = menu_readString(stdin);

    int tmp;
    while (1) {
        printf("Introduzir Peso do Artigo (em gramas) (%lu) $ ", a->peso_gramas);
        if (menu_readInt(&tmp)) {
            if(tmp < 0) menu_printError("artigo não pode ter peso negativo");
            else { a->peso_gramas = tmp; break; }
        }
    }
    while (1) {
        printf("Introduzir Volume do Artigo (em centimetros cúbicos) (%lu) $ ", a->cmCubicos);
        if (menu_readInt(&tmp)) {
            if(tmp < 0) menu_printError("artigo não pode ter volume negativo");
            else { a->cmCubicos = tmp; break; }
        }
    }
    return 1;
}

void funcional_editar_artigos(artigovec* ar) {
    while (1) {
        menu_printInfo("numero de artigos atuais: %lu", ar->size);
        menu_printHeader("Selecionar Artigo Para Editar");
        int op = -2;
        int max = ar->size+1;
        while(op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            int i = -1;
            artigovec_iterateFW(ar, (artigovec_predicate_t)&vecPrintArtigoPredicate, (void*)&i);
            printf("   %*d   |   Criar Novo Artigo\n", 8, ++i);
            while (!menu_readIntMinMax(-2, max, &op));
            menu_printDiv();
        }
        // Option was gotten -------------------------
        if(op == -1) return;
        if(op == max-1) artigovec_push(ar, newArtigo());
        if(!funcional_editar_artigo(&(ar->data[op]), (op != max - 1))) {
            freeArtigo(&(ar->data[op]));
            artigovec_moveBelow(ar, op);
            menu_printInfo("Artigo removido");
        }
    }
}

void interface_criar_encomenda() {
    menu_printDiv();
    menu_printHeader("Criar Nova Encomenda");
    if(artigos.size != 0) {
        menu_printInfo("encomenda por formalizar encontrada, o que deseja fazer?");
        switch ( menu_selection(&(strvec){
            .size = 2,
            .data = (char*[]) {
                "Descartar encomenda anterior",
                "Continuar encomenda anterior"
            }
        }) ) {
            case -1: return;
            case  0:
                artigovec_free(&artigos);
                artigos = artigovec_new();
            break;
            case  1: break;
        }
    }
    funcional_editar_artigos(&artigos);
    menu_printInfo("a formalizar encomenda");
    if(artigos.size == 0) {
        menu_printError("impossivel formalizar encomenda sem artigos");
        return;
    }

    encomenda e;
    if(!funcional_formalizar_encomenda(&artigos, &e)) {
        menu_printInfo("encomenda não formalizada.");
    } else {
        encomendavec_push(&encomendas, e);
        menu_printInfo("encomenda adicionada com sucesso!");
    }
}

void interface_editar_encomendas() {
    menu_printDiv();
    menu_printHeader("Selecione Encomenda Para Editar");
    int op = -2;
    int max = encomendas.size - 1;
    while(op == -2) {
        printf("   Opção      |   Item\n");
        printf("         -2   |   Reimprimir\n");
        printf("         -1   |   Sair\n");
        funcional_consultar_estados_encomendas();
        while (!menu_readIntMinMax(-2, max, &op));
        menu_printDiv();
    }
    if(op == -1) return;
    encomenda* e = &(encomendas.data[op]);
    menu_printEncomendaDetail(e);

    if (e->tipoEstado & ENCOMENDA_ESTADO_EM_ENTREGA) {
        // Encomenda está por entregar
        while (1) {
            menu_printHeader("Encomenda Por Entregar");
            switch (menu_selection(& (strvec) {
                .data = (char*[]){
                    "Editar artigos da encomenda",
                    "Cancelar encomenda",
                    "Definir encomenda como urgente",
                    "Definir encomenda como fragil",
                    "Alterar morada de origem",
                    "Alterar morada de destino"
                },
                .size = 6
            })) {
                case -1: return;
                case  0: {
                    funcional_editar_artigos(&(e->artigos));
                    menu_printInfo("a formalizar encomenda");
                    e->tipoEstado = ENCOMENDA_ESTADO_EM_ENTREGA;
                    if(e->artigos.size == 0) {
                        menu_printError("impossivel formalizar encomenda sem artigos, a cancelar encomenda");
                        encomenda_ESTADO_CANCELADA(e);
                    }
                    encomenda_TIPO_VOLUMOSO(e);
                    encomenda_TIPO_FRAGIL(e);
                    encomenda_TIPO_PESADO(e);
                    return;
                }
                case  1: encomenda_ESTADO_CANCELADA(e);             return;
                case  2: encomenda_TIPO_URGENTE(e);                 break;
                case  3: encomenda_TIPO_FRAGIL_togle(e);            break;
                case  4: interface_editar_morada(&(e->origem), 0);  break;
                case  5: interface_editar_morada(&(e->destino), 0); break;
            }
        }
    } else if (e->tipoEstado & ENCOMENDA_ESTADO_CANCELADA) {
        menu_printHeader("Encomenda Cancelada");
        switch (menu_selection(& (strvec) {
            .data = (char*[]){
                "Reativar encomenda"
            },
            .size = 1
        })) {
            case -1: return;
            case  0: {
                if(e->artigos.size == 0) {
                    menu_printError("impossivel reativar encomenda sem artigos");
                } else {
                    encomenda_ESTADO_CANCELADA(e);
                }
                return;
            }
        }
    } else if (e->tipoEstado & ENCOMENDA_ESTADO_ENTREGUE) {
        menu_printError("a encomenda foi entregue, não pode ser editada");
    } else if (e->tipoEstado & ENCOMENDA_ESTADO_EXPEDIDA) {
        menu_printError("a encomenda foi expedida, não pode ser editada");
    }
}

void interface_cliente() {
    while (1) {
        if(utilizadorAtual == UTILIZADOR_INVALIDO) return;
        menu_printDiv();
        menu_printHeader("Menu de Cliente");
        strvec vetorOp = (strvec){
            .data = (char*[]){
                "Editar perfil",
                "Remover perfil",
                "Criar nova encomenda",
                "Consultar estado de encomendas",
                "Consultar tabela de preços",
                "Editar encomendas",
                "Imprimir recibo de encomenda",
                "Imprimir recibo de um mês"
            },
            .size = 7
        };
        switch (menu_selection(&vetorOp)) {
            case -1: return;
            case  0: funcional_editar_utilizador(utilizadorAtual);     break;
            case  1: funcional_desativar_perfil(); break;
            case  2: interface_criar_encomenda();      break;
            case  3: menu_printDiv(); menu_printHeader("Encomendas"); funcional_consultar_estados_encomendas(); break;
            case  4: funcional_consultar_tabela_de_precos(1, 1); break;
            case  5: interface_editar_encomendas(); break;
            case  6: interface_imprimir_recibo(); break;
        }
    }
}

void interface_novoRegisto() {
    menu_printDiv();
    menu_printHeader("Novo Registo");
    if(utilizadores.size != 0) {
        menu_printInfo("ainda tem um registo aberto, tem a certeza que quer descartar os dados do registo?");
        switch (menu_selection(&(strvec){
            .data = (char*[]){
                "Descartar registo"
            },
            .size = 1
        })) {
            case -1: return;
            case  0: break;
        }
    }

    artigovec_free     (&artigos);
    encomendavec_free  (&encomendas);
    utilizadorvec_free (&utilizadores);
    artigos      = artigovec_new();
    encomendas   = encomendavec_new();
    utilizadores = utilizadorvec_new();

    menu_printHeader("Registar Novo Diretor");

    utilizadorvec_push(&utilizadores, newUtilizador());
    funcional_editar_diretor(0);
    menu_printInfo("diretor criado com sucesso!");
}

void funcional_carregarDados() {
    artigovec_free     (&artigos);
    encomendavec_free  (&encomendas);
    utilizadorvec_free (&utilizadores);

    artigos      = artigovec_new();
    encomendas   = encomendavec_new();
    utilizadores = utilizadorvec_new();

    FILE* f = fopen("data.bin", "rb");
    if(!f) {
        menu_printError("impossivel abrir ficheiro :%d", errno);
        return;
    }
    if(!load_precos(f, &tabelaPrecos)) {
        menu_printError("impossivel carregar tabela de preços :%d", errno);
        return;
    }
    uint64_t size_tmp = 0;
    if(!fread(&(size_tmp), sizeof(uint64_t), 1, f)) {
        menu_printError("impossivel ler tamanho de utilizadores :%d", errno);
        return;
    }
    utilizadorvec_reserve(&utilizadores, size_tmp);
    for(uint64_t i = 0; i < size_tmp; ++i) {
        ++utilizadores.size;
        if(!load_utilizador(f, &(utilizadores.data[i]))) {
            menu_printError("impossivel carregar utilizador [%lu] :%d", i, errno);
            --utilizadores.size;
            return;
        }
    }
    if(!fread(&(size_tmp), sizeof(uint64_t), 1, f)) {
        menu_printError("impossivel carregar tamanho de encomendas :%d", errno);
        return;
    }
    encomendavec_reserve(&encomendas, size_tmp);
    for(uint64_t i = 0; i < size_tmp; ++i) {
        ++encomendas.size;
        if(!load_encomenda(f, &(encomendas.data[i]))) {
            menu_printError("impossivel carregar encomenda [%lu] :%d", i, errno);
            --encomendas.size;
            return;
        }
    }
    menu_printInfo("Dados carregados com sucesso!");
    fclose(f);
}

void interface_registoUtilizador() {
    menu_printDiv();
    menu_printHeader("Registo de Novo Utilizador");

    while (utilizadores.size == 0) {
        menu_printInfo("ainda não tem um registo aberto, quer criar um novo registo?");
        switch (menu_selection(&(strvec){
            .data = (char*[]){
                "Criar um novo registo",
                "Carregar registo de ficheiro"
                },
            .size = 2
        })) {
            case -1: return;
            case  0: interface_novoRegisto();   return;
            case  1: funcional_carregarDados(); return;
        }
    }

    menu_printHeader("Registar Novo Utilizador");
    utilizador u = newUtilizador();

    printf("Introduzir Nome $ ");
    freeCHK(u.nome, menu_readString(stdin));

    char* stmp = NULL;
    while (1) {
        printf("Introduzir NIF ");
        freeN(stmp);
        while(!stmp) { printf("$ "); stmp = menu_readString(stdin); }
        if (strlen(stmp) != 9) {
            menu_printError("tem que introduzir 9 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eNIFValido((uint8_t*)stmp)) {
            menu_printError("NIF introduzido é inválido.");
        } else {
            size_t index = utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_predicate_t)&utilizadorNIFCompareVecPredicate, stmp);
            if(index != (~((size_t)0))) {
                menu_printError("NIF já existente no sistema.");
                return;
            } else {
                memcpy(&u.NIF, stmp, sizeof(uint8_t)*9);
                break;
            }
        }
    }

    printf("Introduzir Morada $ ");
    freeCHK(u.endereco.morada, menu_readString(stdin));

    while (1) {
        printf("Introduzir Código Postal (XXXX-XXX) ");
        freeN(stmp);
        while(!stmp) { printf("$ "); stmp = menu_readString(stdin); }
        if (strlen(stmp) != 8) {
            menu_printError("tem que introduzir 8 characteres, %d introduzidos.", strlen(stmp));
        } else {
            u.endereco.codigoPostal[0] = stmp[0];
            u.endereco.codigoPostal[1] = stmp[1];
            u.endereco.codigoPostal[2] = stmp[2];
            u.endereco.codigoPostal[3] = stmp[3];
            u.endereco.codigoPostal[4] = stmp[5];
            u.endereco.codigoPostal[5] = stmp[6];
            u.endereco.codigoPostal[6] = stmp[7];
            if (!morada_eCPValido(u.endereco.codigoPostal)) {
                menu_printError("Código Postal introduzido é inválido.");
            } else break;
        }
    }

    while (1) {
        printf("Introduzir numero do cartao de cidadão $ ");
        freeN(stmp);
        while(!stmp) { printf("$ "); stmp = menu_readString(stdin); }
        if (strlen(stmp) != 12) {
            menu_printError("tem que introduzir 12 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eCCValido((uint8_t*)stmp)) {
            menu_printError("CC introduzido é inválido.");
        } else {
            memcpy(&u.CC, stmp, sizeof(uint8_t)*12);
            break;
        }
    }
    free(stmp);

    u.tipo = UTILIZADOR_CLIENTE;
    utilizadorvec_push(&utilizadores, u);
    menu_printInfo("utilizador %s (%.9s) adicionado com sucesso!", u.nome, u.NIF);
}

void interface_login() {
    char* stmp = NULL;
    while (1) {
        utilizadorAtual = UTILIZADOR_INVALIDO;
        if(artigos.size != 0) {
            menu_printInfo("encomenda por formalizar foi eleminada");
            artigovec_free(&artigos);
            artigos = artigovec_new();
        }
        menu_printDiv();
        menu_printHeader("Efetuar Login ?");
        switch (menu_selection(&(strvec){
            .data = (char*[]){"Efetuar login"},
            .size = 1
        })) {
            case -1: freeN(stmp); return;
        }
        printf("Introduzir NIF $ ");
        freeN(stmp);
        while(!stmp) { printf("$ "); stmp = menu_readString(stdin); }
        if (strlen(stmp) != 9) {
            menu_printError("tem que introduzir 9 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eNIFValido((uint8_t*)stmp)) {
            menu_printError("NIF introduzido é inválido.");
        } else {
            size_t index = utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_predicate_t)&utilizadorAtivadoNIFCompareVecPredicate, stmp);
            if(index == UTILIZADOR_INVALIDO) {
                menu_printError("NIF não reconhecido como um utilizador.");
            } else {
                switch (utilizadores.data[index].tipo) {
                    case UTILIZADOR_DESATIVADO:
                        menu_printError("o seu prefil está desativado, contacte um diretor para o ativar");
                    break;
                    case UTILIZADOR_CLIENTE:
                        menu_printInfo("login efetuado com sucesso.");
                        utilizadorAtual = index;
                        interface_cliente();
                    break;
                    case UTILIZADOR_DIRETOR:
                        menu_printInfo("login efetuado como diretor.");
                        utilizadorAtual = index;
                        interface_diretor();
                    break;
                    default:
                        menu_printError("utilizador invalido!");
                    break;
                }
            }
        }
    }
}

/*
    precos_tt_cent tabelaPrecos;
    encomendavec   encomendas;
    utilizadorvec  utilizadores;

    para guardar str (char*)
        * uint32_t tamanho
        * char* data[]
        (SE tamanho == 0, retornar NULL ao ler)

    Estrutura do ficheiro:
        - precos_tt_cent tabela_de_precos
            * uint64_t REGULAR
            * uint64_t URGENTE
            * uint64_t VOLUMOSO
            * uint64_t FRAGIL
            * uint64_t PESADO
            * uint64_t POR_KM
            - _Float32[100] MULT_CP
                * _Float32 (32b) de [0][0], [0][1] .. [0][9], [1][0] .. [9][9]
        * uint64_t tamanho_de_utilizadorvec
        - utilizador utilizadores[tamanho_de_utilizadorvec]
                - str nome
                * uint8_t NIF[9]
                * uint8_t CC[12]
                - morada endereco
                    - str morada
                    * uint8_t codigoPostal[7]
                * uint8_t tipo
        * uint64_t tamanho_de_encomendavec
        - encomenda encomendas[tamanho_de_encomendavec]
                - artigovec      artigos
                    * uint64_t  tamanho_de_artigovec
                    - artigo artigos[amanho_de_artigovec]
                            - str    nome
                            - str    tratamentoEspecial
                            * uint64_t peso_gramas
                            * uint64_t cmCubicos
                - morada         origem
                - morada         destino
                * uint64_t       distancia_km
                * uint8_t        tipoEstado
                - precos_tt_cent precos
                * uint8_t        NIF_cliente[9]
*/

void funcional_guardarDados() {
    FILE* f = fopen("data.bin", "wb");
    if(!f) {
        menu_printError("impossivel abrir ficheiro :%d", errno);
        return;
    }
    if(!save_precos(f, &tabelaPrecos)) {
        menu_printError("impossivel guardar tabela de preços :%d", errno);
        return;
    }
    if(!fwrite(&(utilizadores.size), sizeof(uint64_t), 1, f)) {
        menu_printError("impossivel guardar tamanho de utilizadores :%d", errno);
        return;
    }
    for(uint64_t i = 0; i < utilizadores.size; ++i) {
        if(!save_utilizador(f, &(utilizadores.data[i]))) {
            menu_printError("impossivel guardar utilizador [%lu] :%d", i, errno);
            return;
        }
    }
    if(!fwrite(&(encomendas.size), sizeof(uint64_t), 1, f)) {
        menu_printError("impossivel guardar tamanho de encomendas :%d", errno);
        return;
    }
    for(uint64_t i = 0; i < encomendas.size; ++i) {
        if(!save_encomenda(f, &(encomendas.data[i]))) {
            menu_printError("impossivel guardar encomenda [%lu] :%d", i, errno);
            return;
        }
    }
    menu_printInfo("Dados guardados com sucesso!");
    fclose(f);
}

void interface_inicio() {
    while (1) {
        menu_printDiv();
        menu_printHeader("Início");
        switch (menu_selection(&(strvec){
            .size = 5,
            .data = (char*[]){"Log In", "Registar utilizador", "Carregar dados", "Guardar dados", "Novo registo"}
        })) {
            case -1: return;
            case  0: interface_login             (); break;
            case  1: interface_registoUtilizador (); break;
            case  2: funcional_carregarDados     (); break;
            case  3: funcional_guardarDados      (); break;
            case  4: interface_novoRegisto       (); break;
        }
    }
}

int main() {
    // TODO: apagar, apenas util para debug
    setvbuf(stdout, NULL, _IONBF, 0);

    menu_printDiv();
    menu_printHeader("A Iniciar");
    artigos      = artigovec_new();
    encomendas   = encomendavec_new();
    utilizadores = utilizadorvec_new();

    interface_inicio();
    menu_printHeader("A Terminar");

    artigovec_free     (&artigos);
    encomendavec_free  (&encomendas);
    utilizadorvec_free (&utilizadores);
    menu_printDiv();

    return 0;
}