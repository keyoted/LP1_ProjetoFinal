// TODO: apagar, apenas util para debug
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#define UTILIZADOR_INVALIDO ~((size_t)0)
#define  VEC_IMPLEMENTATION
#include "menu.h"
#include "artigo.h"
#include "encomenda.h"
#include "utilizador.h"

#ifndef artigovec_H
#define artigovec_H
#define VEC_TYPE         artigo
#define VEC_NAME         artigovec
#define VEC_DEALOC(X)    freeArtigo(&X)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#endif

#ifndef encomendavec_H
#define encomendavec_H
#define VEC_TYPE         encomenda
#define VEC_NAME         encomendavec
#define VEC_DEALOC(X)    freeEncomenda(&X)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#endif

#ifndef utilizadorvec_H
#define utilizadorvec_H
#define VEC_TYPE         utilizador
#define VEC_NAME         utilizadorvec
#define VEC_DEALOC(X)    freeUtilizador(&X)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#endif

artigovec      artigos;                                 // Artigos da seção atual, que ainda não foram formalizados numa encomenda
encomendavec   encomendas;                              // Encomendas formalizadas
utilizadorvec  utilizadores;                            // Utilizadores existentes no registo, index 0 é diretor
size_t         utilizadorAtual = UTILIZADOR_INVALIDO;   // Index do utilizador atual
precos_tt_cent tabelaPrecos;                            // Preço em centimos por cada tipo de transporte

void funcional_consultar_estados_encomendas() {
    if(utilizadorAtual == 0) {
        for(size_t i = 0; i < encomendas.size; ++i) {
            printf("   %*lu   |   ", 8, i);
            menu_printEncomendaBrief(encomendas.data[i]);
            printf("\n");
        }
    } else {
        for(size_t i = 0; i < encomendas.size; ++i) {
            if( memcmp(utilizadores.data[utilizadorAtual].NIF, encomendas.data[i].NIF_cliente, 9) == 0 ) {
                printf("   %*lu   |   ", 8, i);
                menu_printEncomendaBrief(encomendas.data[i]);
                printf("\n");
            }
        }
    }
}

void interface_imprimir_recibo() {
    menu_printHeader("Selecionar Encomenda Para Imprimir");
    // Get option ---------------------------------
    int op = -2;
    int max = encomendas.size;
    while(op == -2) {
        printf("   Opção      |   Item\n");
        printf("         -2   |   Reimprimir\n");
        printf("         -1   |   Sair\n");
        funcional_consultar_estados_encomendas();
        while (!menu_readIntMinMax(-2, max-1, &op));
        menu_printDiv();
    }
    // Option was gotten -------------------------
    if(op == -1) return;
    if((utilizadorAtual != 0) && (memcmp(utilizadores.data[utilizadorAtual].NIF, encomendas.data[op].NIF_cliente, 9) != 0) ) {
        menu_printError("não tem premissões para imprimir este recibo!");
        return;
    }
    menu_printEncomendaDetail(encomendas.data[op]);
}

morada interface_editar_morada(morada* const morg) {
    morada m;
    if( morg==NULL ) m = newMorada();
    else             m = (*morg);

    if( morg == NULL) printf("Introduzir Morada $ ");
    else              printf("Introduzir Morada (%s) $ ", m.morada);

    if(m.morada) free(m.morada);
    m.morada = menu_readString(stdin);

    char* stmp = NULL;
    do {
        if( morg == NULL ) printf("Introduzir Código Postal (XXXX-XXX) $ ");
        else               printf("Introduzir Código Postal (XXXX-XXX) (%c%c%c%c-%c%c%c) $ ", m.codigoPostal[0], m.codigoPostal[1], m.codigoPostal[2], m.codigoPostal[3], m.codigoPostal[4], m.codigoPostal[5], m.codigoPostal[6]);

        if(stmp) freeN(stmp);
        stmp = menu_readString(stdin);

        if (strlen(stmp) != 8) {
            menu_printError("tem que introduzir 8 characteres, %d introduzidos.", strlen(stmp));
        } else {
            m.codigoPostal[0] = stmp[0];
            m.codigoPostal[1] = stmp[1];
            m.codigoPostal[2] = stmp[2];
            m.codigoPostal[3] = stmp[3];
            m.codigoPostal[4] = stmp[5];
            m.codigoPostal[5] = stmp[6];
            m.codigoPostal[6] = stmp[7];
            if (!morada_eCPValido(m.codigoPostal)) {
                menu_printError("Código Postal introduzido é inválido.");
            } else break;
        }
    } while (1);
    free(stmp);

    return m;
}

int utilizadorAtivadoNIFCompareVecPredicate(utilizador element, uint8_t* compare) {
    if(element.tipo == UTILIZADOR_DESATIVADO) return 0;
    return memcmp(element.NIF, compare, sizeof(uint8_t) * 9) == 0;
}

void interface_editar_diretor() {
    menu_printDiv();
    menu_printHeader("Editar Diretor");

    freeUtilizador(&(utilizadores.data[0]));
    utilizadores.data[0] = newUtilizador();
    utilizador* diretor = & utilizadores.data[0];
    diretor->tipo = UTILIZADOR_DIRETOR;

    printf("Introduzir Nome $ ");
    if(diretor->nome) free(diretor->nome);
    diretor->nome = menu_readString(stdin);

    char* stmp = NULL;
    do {
        printf("Introduzir NIF $ ");
        if(stmp) freeN(stmp);
        stmp = menu_readString(stdin);
        if (strlen(stmp) != 9) {
            menu_printError("tem que introduzir 9 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eNIFValido((uint8_t*)stmp)) {
            menu_printError("NIF introduzido é inválido.");
        } else {
            memcpy(&diretor->NIF, stmp, sizeof(uint8_t)*9);
            break;
        }
    } while (1);
    free(stmp);
    menu_printInfo("diretor editado com sucesso!");
}

void interface_editar_utilizador(size_t index) {
    menu_printDiv();
    menu_printHeader("Edição de Utilizador");
    utilizador u = utilizadores.data[utilizadorAtual];

    printf("Introduzir Nome (%s) $ ", u.nome);
    if(u.nome) free(u.nome);
    u.nome = menu_readString(stdin);

    u.endereco = interface_editar_morada(&(u.endereco));

    char* stmp = NULL;
    do {
        printf("Introduzir numero do cartao de cidadão (%c%c%c%c%c%c%c%c%c%c%c%c) $ ", u.CC[0], u.CC[1], u.CC[2], u.CC[3], u.CC[4], u.CC[5], u.CC[6], u.CC[7], u.CC[8], u.CC[9], u.CC[10], u.CC[11]);
        if(stmp) freeN(stmp);
        stmp = menu_readString(stdin);
        if (strlen(stmp) != 12) {
            menu_printError("tem que introduzir 12 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eCCValido((uint8_t*)stmp)) {
            menu_printError("CC introduzido é inválido.");
        } else {
            memcpy(&u.CC, stmp, sizeof(uint8_t)*12);
            break;
        }
    } while (1);
    free(stmp);

    u.tipo = UTILIZADOR_CLIENTE;

    utilizadores.data[utilizadorAtual]=u;

    menu_printInfo("utilizador %s (%.9s) editado com sucesso!", u.nome, u.NIF);
}

void funcional_consultar_tabela_de_precos(int printDist, int printTable) {
    if (printDist) {
        menu_printHeader("Tabela de Distancia");
        printf("ORIGEM DESTINO  0           1           2           3           4           5           6           7           8           9\n");
        for (int origem = 0; origem < 10; ++origem) {
            printf("  %d  |        ", origem);
            for (int destino = 0; destino < 10; ++destino) {
                printf("  %07.4f  |", (double)tabelaPrecos.MULT_CP[origem][destino]);
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
            "Editar preço elemento",
            "Editar preço linha (todos os destinos de uma origem)",
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
                } else{
                    tabelaPrecos.MULT_CP[origem][destino] = novoVal;
                    break;
                }
            }
        }
    } else {
        for (destino = 0; destino < 10; ++destino) {
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

int vecPrintUserPredicate (utilizador item, int* userdata) {
    printf("   %*d   |   ", 8, ++(*userdata));
    menu_printUtilizador(item);
    printf("\n");
    return 0;
}

void interface_alterar_utilizadores(){
    while (1) {
        menu_printDiv();
        menu_printHeader("Selecionar Utilizador Para Editar");
        // Get option ---------------------------------
        int op = -2;
        int max = utilizadores.size;
        while(op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            int i = -1;
            utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_predicate_t)&vecPrintUserPredicate, (void*)&i);
            while (!menu_readIntMinMax(-2, max-1, &op));
            menu_printDiv();
        }
        // Option was gotten -------------------------
        if(op == -1) return;

        menu_printDiv();
        menu_printHeader("Selecionar operação");
        printf("Utilizador selecionado:");
        menu_printUtilizador(utilizadores.data[op]);
        int fnc = menu_selection(&(strvec){
        .data = (char*[]){
            "Ativar utilizador",
            "Desativar utilizador",
            "Editar utilizador"},
        .size = 3
        });
        if(fnc == 0) {
            if(op==0) {menu_printError("Diretor está sempre ativo"); break; }
            utilizadores.data[op].tipo == UTILIZADOR_CLIENTE;
        }
        else if(fnc == 1) {
            if(op==0) {menu_printError("Diretor está sempre ativo"); break; }
            utilizadores.data[op].tipo == UTILIZADOR_DESATIVADO;
        }
        else if(fnc == 2) {
            if(op==0) interface_editar_diretor();
            else interface_editar_utilizador(op);
        }
    }
}

int vecPrintEncomendaPredicate (encomenda item, int* userdata) {
    printf("   %*d   |   ", 8, ++(*userdata));
    menu_printEncomendaBrief(item);
    printf("\n");
    return 0;
}

void interface_editar_estados_encomendas(){
    while (1) {
        menu_printDiv();
        menu_printHeader("Selecionar Encomenda Para Editar");
        // Get option ---------------------------------
        int op = -2;
        int max = encomendas.size;
        while(op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            int i = -1;
            encomendavec_iterateFW(&encomendas, (encomendavec_predicate_t)&vecPrintEncomendaPredicate, (void*)&i);
            while (!menu_readIntMinMax(-2, max-1, &op));
            menu_printDiv();
        }
        // Option was gotten -------------------------
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
                "Alterar tabela de preços",
                "Alterar tabela de distancias",
                "Alterar estado dos utilizadores",
                "Alterar estados das encomendas",
                "Imprimir recibo de encomenda"
            },
            .size = 5
        })) {
            case -1: return;
            case  0: interface_alterar_tabela_precos();     break;
            case  1: interface_alterar_tabela_distancias(); break;
            case  2: interface_alterar_utilizadores();      break;
            case  3: interface_editar_estados_encomendas(); break;
            case  4: interface_imprimir_recibo(); break;
        }
    }
}

void funcional_desativar_perfil(){
    utilizadores.data[utilizadorAtual].tipo=UTILIZADOR_DESATIVADO;
    utilizadorAtual= ~0;
    menu_printInfo("utilizador desativado com sucesso!!");
}

// Artigos size is assumed to be diffrent than 0
encomenda interface_formalizar_encomenda(artigovec* artigos) {
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
        case -1: return newEncomenda();
        case  0: dest = morada_dup(utilizadores.data[utilizadorAtual].endereco); break;
        case  1: dest = interface_editar_morada(NULL); break;
    }

    menu_printInfo("introduzir morada de origem.");
    morada org = interface_editar_morada(NULL);

    int dist;
    while (1) {
        printf("Introduzir Distancia (em Km) $ ");
        menu_readInt(&dist);
        if(dist < 0) menu_printError("distancia negativa não é possivél.");
        else break;
    }
    encomenda e = encomenda_formalizar(*artigos, tabelaPrecos, utilizadores.data[utilizadorAtual].NIF, org, dest, dist);
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
            case  0: encomenda_TIPO_URGENTE(&e);      break;
            case  1: encomenda_TIPO_FRAGIL_togle(&e); break;
            case  2: goto END_OF_LOOP;
        }
    }
    END_OF_LOOP:
    *artigos = artigovec_new();
    menu_printInfo("encomenda formalizada com sucesso!");
    return e;
}

int vecPrintArtigoPredicate (artigo item, int* userdata) {
    printf("   %*d   |   ", 8, ++(*userdata));
    menu_printArtigo(item);
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
    free(a->nome);
    a->nome = menu_readString(stdin);

    if(a->tratamentoEspecial) {
        printf("Introduzir Tratamento especial para o artigo Artigo (%s) $ ", a->tratamentoEspecial);
        freeN(a->tratamentoEspecial);
    } else printf("Introduzir Tratamento especial para o artigo Artigo $ ");
    a->tratamentoEspecial = menu_readString(stdin);
    if( *(a->tratamentoEspecial) == '\0') {
        // Sem tratamento especial
        freeN(a->tratamentoEspecial);
    }

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
        // Get option ---------------------------------
        int op = -2;
        int max = ar->size;
        while(op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            int i = -1;
            artigovec_iterateFW(ar, (artigovec_predicate_t)&vecPrintArtigoPredicate, (void*)&i);
            printf("   %*d   |   Criar Novo Artigo\n", 8, ++i);
            while (!menu_readIntMinMax(-2, max+1, &op));
            menu_printDiv();
        }
        // Option was gotten -------------------------
        if(op == -1) return;
        if(op == max) {
            artigovec_push(ar, newArtigo());
        }
        if(!funcional_editar_artigo(&(ar->data[op]), (op != max))) {
            freeArtigo(&(ar->data[op]));
            artigovec_moveBelow(ar, op);
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
            case  0: {
                artigovec_free(&artigos);
                artigos = artigovec_new();
                break;
            }
            case  1: break;
        }
    }
    funcional_editar_artigos(&artigos);
    menu_printInfo("a formalizar encomenda");
    if(artigos.size == 0) {
        menu_printError("impossivel formalizar encomenda sem artigos");
        return;
    }
    encomenda e = interface_formalizar_encomenda(&artigos);
    if(e.artigos.size == 0) {
        menu_printInfo("encomenda não formalizada.");
        freeEncomenda(&e);
    } else {
        encomendavec_push(&encomendas, e);
        menu_printInfo("encomenda adicionada com sucesso!");
    }
}

void interface_editar_encomendas() {
    menu_printDiv();
    menu_printHeader("Selecione Encomenda Para Editar");
    // Get option ---------------------------------
    int op = -2;
    int max = encomendas.size;
    while(op == -2) {
        printf("   Opção      |   Item\n");
        printf("         -2   |   Reimprimir\n");
        printf("         -1   |   Sair\n");
        funcional_consultar_estados_encomendas();
        while (!menu_readIntMinMax(-2, max-1, &op));
        menu_printDiv();
    }
    // Option was gotten -------------------------
    if(op == -1) return;
    encomenda* e = &(encomendas.data[op]);
    menu_printEncomendaDetail(*e);
    if (e->tipoEstado & ENCOMENDA_ESTADO_EM_ENTREGA) {
        // Encomenda está por entregar
        while (1) {
            menu_printHeader("Encomenda Por Entregar");
            switch (menu_selection(& (strvec) {
                .data = (char*[]){
                    "Editar artigos da encomenda",
                    "Cancelar encomenda",
                    "Definir encomenda como urgente",
                    "Definir encomenda como fragil"
                },
                .size = 4
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
                case  1: encomenda_ESTADO_CANCELADA(e); return;
                case  2: encomenda_TIPO_URGENTE(e); break;
                case  3: encomenda_TIPO_FRAGIL_togle(e); break;
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
        if(utilizadorAtual==UTILIZADOR_INVALIDO) return;
        menu_printDiv();
        menu_printHeader("Menu de Cliente");
        strvec vetorOp = (strvec){
            .data = (char*[]){
                "Editar perfil",
                "Remover perfil",
                "Criar nova encomenda",
                "Consultar estado de encomendas",
                "Consultar tabela de preços",
                "Imprimir recibo de encomenda",
                "Editar encomendas"
            },
            .size = 7
        };
        switch (menu_selection(&vetorOp)) {
            case -1: return;
            case  0: interface_editar_utilizador(utilizadorAtual);     break;
            case  1: funcional_desativar_perfil(); break;
            case  2: interface_criar_encomenda();      break;
            case  3: menu_printDiv(); menu_printHeader("Encomendas"); funcional_consultar_estados_encomendas(); break;
            case  4: funcional_consultar_tabela_de_precos(1, 1); break;
            case  5: interface_imprimir_recibo(); break;
            case  6: interface_editar_encomendas(); break;
        }
    }
}

void interface_novoRegisto();
void funcional_carregarDados();

void interface_registoUtilizador() {
    menu_printDiv();
    menu_printHeader("Registo de Novo Utilizador");

    while (utilizadores.size == 0) {
        menu_printInfo("ainda não tem um registo aberto, quer criar um novo registo?");
        switch (menu_selection(&(strvec){
            .data = (char*[]){"Criar um novo registo", "Carregar registo de ficheiro"},
            .size = 2
        })) {
            case -1: return;
            case  0: interface_novoRegisto();   break;
            case  1: funcional_carregarDados(); break;
        }
    }

    menu_printHeader("Registar Novo Utilizador");
    utilizador u = newUtilizador();

    printf("Introduzir Nome $ ");
    if(u.nome) free(u.nome);
    u.nome = menu_readString(stdin);

    char* stmp = NULL;
    do {
        printf("Introduzir NIF $ ");
        if(stmp) freeN(stmp);
        stmp = menu_readString(stdin);
        if (strlen(stmp) != 9) {
            menu_printError("tem que introduzir 9 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eNIFValido((uint8_t*)stmp)) {
            menu_printError("NIF introduzido é inválido.");
        } else {
            size_t index = utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_predicate_t)&utilizadorAtivadoNIFCompareVecPredicate, stmp);
            if(index != (~((size_t)0))) {
                menu_printError("NIF já existente no sistema.");
            } else {
                memcpy(&u.NIF, stmp, sizeof(uint8_t)*9);
                break;
            }
        }
    } while (1);

    printf("Introduzir Morada $ ");
    free(u.endereco.morada);
    u.endereco.morada = menu_readString(stdin);

    if(stmp) freeN(stmp);
    do {
        printf("Introduzir Código Postal (XXXX-XXX) $ ");
        if(stmp) freeN(stmp);
        stmp = menu_readString(stdin);
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
    } while (1);

    do {
        printf("Introduzir numero do cartao de cidadão $ ");
        if(stmp) freeN(stmp);
        stmp = menu_readString(stdin);
        if (strlen(stmp) != 12) {
            menu_printError("tem que introduzir 12 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eCCValido((uint8_t*)stmp)) {
            menu_printError("CC introduzido é inválido.");
        } else {
            memcpy(&u.CC, stmp, sizeof(uint8_t)*12);
            break;
        }
    } while (1);
    free(stmp);

    u.tipo = UTILIZADOR_CLIENTE;

    utilizadorvec_push(&utilizadores, u);

    menu_printInfo("utilizador %s (%.9s) adicionado com sucesso!", u.nome, u.NIF);
}

void interface_novoRegisto() {
    menu_printDiv();
    menu_printHeader("Novo Registo");
    if(utilizadores.size != 0) {
        menu_printInfo("ainda tem um registo aberto, tem a certeza que quer descartar os dados do registo?");
        switch (menu_selection(&(strvec){
            .data = (char*[]){"Descartar registo"},
            .size = 1
        })) {
            case -1: return;
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
    interface_editar_diretor();
    menu_printInfo("diretor criado com sucesso!");
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
            case -1: if(stmp) freeN(stmp); return;
        }
        printf("Introduzir NIF $ ");
        if(stmp) freeN(stmp);
        stmp = menu_readString(stdin);
        if (strlen(stmp) != 9) {
            menu_printError("tem que introduzir 9 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eNIFValido((uint8_t*)stmp)) {
            menu_printError("NIF introduzido é inválido.");
        } else {
            size_t index = utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_predicate_t)&utilizadorAtivadoNIFCompareVecPredicate, stmp);
            if(index == 0) {
                menu_printInfo("login efetuado como diretor.");
                utilizadorAtual = 0;
                interface_diretor();
            } else if ( index != (~((size_t)0)) ) {
                menu_printInfo("login efetuado com sucesso.");
                utilizadorAtual = index;
                interface_cliente();
            } else {
                menu_printError("NIF não reconhecido como um utilizador.");
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
    // TODO: Implement
}

void funcional_carregarDados() {
    // TODO: Implement
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

    artigos      = artigovec_new();
    encomendas   = encomendavec_new();
    utilizadores = utilizadorvec_new();

    menu_printHeader("A Iniciar");
    interface_inicio();
    menu_printHeader("A Terminar");

    artigovec_free     (&artigos);
    encomendavec_free  (&encomendas);
    utilizadorvec_free (&utilizadores);
    return 0;
}