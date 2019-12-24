// TODO: apagar, apenas util para debug
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#ifndef __STDC_IEC_559__
_Static_assert(0, "Float may not be 32bits");
#endif

#define UTILIZADOR_INVALIDO ~((size_t)0)
#define  VEC_IMPLEMENTATION
#include "menu.h"

/*
    As empresas de distribuição e logística representam uma atividade muito
    importante no atual sistema económico. Um forte impulsionador terá sido a
    possibilidade de comprar os mais variados artigos pela Internet e aguardar
    comodamente a sua chegada numa morada definida. O cálculodos custos de
    transporte variam de acordo com o tipo de artigo, nomeadamente se este
    carece de cuidados especiais (por exemplo, contém partes frágeis), se é
    volumosoou se é de caráter urgente.
    Um cliente no sistema representa uma pessoa/entidade que pretende usufruir
    dos serviços de transporte. Este insere todos os dados referentes à
    encomenda que pretende enviar e obtém o custo de envio.
    O Diretor é a entidade máxima da empresa de transportes que tem acesso a
    toda a informação de todos os clientes e encomendas transportadas.
    Pode ainda alterar preços e verificar o estado da faturação de cada mês,
    entre outros. Neste sistema considera-se que o sistema de login apenas
    requer o NIF para identificaro utilizador como Cliente ou Diretor, não sendo
    necessário validar passwords.

    Pretende-se o desenvolvimento de um sistema de informação básico para gerir
    asoperações de logística de uma empresa de distribuição.

    Apresentam-se de seguida os conceitos mais relevantes:
*/

/*
    Artigo -
    Representa um objeto que será transportado. Este pode requerer tratamento
    especial(definido de forma textual dentro do artigo), e necessita da
    definição de peso e volume. Um artigo com peso superior a 20 quilogramas
    deverá ser automaticamente considerado Artigo Pesado.
    Se o volume for superior a 0,125 metros cúbicos (equivalente a  uma caixa
    quadradade lado 50 centímetros) então deve ser considerado volumoso;
*/

#include "artigo.h"

/*
    Encomenda -
    Constitui um conjunto de artigos que serão transportados em conjunto desde
    uma determinada origem até ao seu destino. A encomenda deverá ter associado
    o tipo de entrega, ou seja, identificar se é consideradacomo urgente ou
    serviço regular;
*/

/*
    Custo de transporte -
    Representao valor final que o cliente terá de pagar à empresa transportadora
    de acordo com o tipo de encomenda, a distância percorrida (em quilómetros),
    e com a região de origem e de destino;
*/

#include "encomenda.h"

/*
    Cliente -
    Representa um cliente que regista o pedido de transporte e fornece as
    informações necessárias sobre a encomenda. Para facilitar o processo de
    envio, a empresa transportadora regista o seu nome, NIF, número de CC, e
    morada de origem por omissão de modo a facilitar o processo de novas
    encomendas;
*/

/*
    Diretor -
    Representa uma pessoa da empresa de transporte com permissão para aceder a
    toda a informação guardada, nomeadamente sobre clientes, transportes
    realizados, entre outros.
*/

#include "utilizador.h"

/*
    Funcionalidades comuns -
    Esta secção descreve o  conjunto de funcionalidades a implementar que são
    comuns a todos os grupos.
    Pretende-se que modele as estruturas de dados e desenvolva as funções
    necessárias para implementarasseguintes funcionalidades:
*/

#ifndef artigovec_H
#define artigovec_H
#define VEC_TYPE         artigo
#define VEC_NAME         artigovec
#define VEC_DEALOC(X)    freeArtigo(X)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#endif

#ifndef encomendavec_H
#define encomendavec_H
#define VEC_TYPE         encomenda
#define VEC_NAME         encomendavec
#define VEC_DEALOC(X)    freeEncomenda(X)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#endif

#ifndef utilizadorvec_H
#define utilizadorvec_H
#define VEC_TYPE         utilizador
#define VEC_NAME         utilizadorvec
#define VEC_DEALOC(X)    freeUtilizador(X)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#endif

artigovec      artigos;                                 // Artigos da seção atual, que ainda não foram formalizados numa encomenda
encomendavec   encomendas;                              // Encomendas formalizadas
utilizadorvec  utilizadores;                            // Utilizadores existentes no registo, index 0 é diretor
size_t         utilizadorAtual = UTILIZADOR_INVALIDO;   // Index do utilizador atual
float          mult_CP[10][10];                         // Mapeia [Origem][Destino]
precos_tt_cent tabelaPrecos;                            // Preço em centimos por cada tipo de transporte

/*
    Gestão de utilizadores -
    Deverá ser possível a um cliente criar, editar e remover o seu perfil de
    utilizador. Note que a remoção de um clientenão deve ter como resultado a
    remoção de toda a informação relativa a esse cliente,mas apenas a
    “marcação” como removido, não permitindo novo acesso, ou introdução de novas
    encomendas. O Diretor pode realizar qualquer uma das operações de
    manipulação de clientes inclusive reativar um cliente removido.
*/

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

        if(stmp) free(stmp);
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

    return m;
}

int utilizadorAtivadoNIFCompareVecPredicate(utilizador element, uint8_t* compare) {
    if(element.tipo == UTILIZADOR_DESATIVADO) return 0;
    return memcmp(element.NIF, compare, sizeof(uint8_t) * 9) == 0;
}

void interface_editar_diretor() {
    menu_printDiv();
    menu_printHeader("Editar Diretor");

    freeUtilizador(utilizadores.data[0]);
    utilizadores.data[0] = newUtilizador();
    utilizador* diretor = & utilizadores.data[0];
    diretor->tipo = UTILIZADOR_DIRETOR;

    printf("Introduzir Nome $ ");
    free(diretor->nome);
    diretor->nome = menu_readString(stdin);

    char* stmp = NULL;
    do {
        printf("Introduzir NIF $ ");
        if(stmp) free(stmp);
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
    menu_printInfo("diretor editado com sucesso!");
}

void interface_editar_utilizador(size_t index) {
    menu_printDiv();
    menu_printHeader("Edição de Utilizador");
    utilizador u = utilizadores.data[utilizadorAtual];

    printf("Introduzir Nome (%s) $ ", u.nome);
    free(u.nome);
    u.nome = menu_readString(stdin);

    u.adereco = interface_editar_morada(&(u.adereco));

    char* stmp = NULL;
    do {
        printf("Introduzir numero do cartao de cidadão (%c%c%c%c%c%c%c%c%c%c%c%c) $ ", u.CC[0], u.CC[1], u.CC[2], u.CC[3], u.CC[4], u.CC[5], u.CC[6], u.CC[7], u.CC[8], u.CC[9], u.CC[10], u.CC[11]);
        if(stmp) free(stmp);
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

    u.tipo = UTILIZADOR_CLIENTE;

    utilizadores.data[utilizadorAtual]=u;

    menu_printInfo("utilizador %s (%.9s) editado com sucesso!", u.nome, u.NIF);
}

void interface_alterar_tabela_precos(){
    //TODO: Implementar
}

void interface_alterar_tabela_distancias(){
    //TODO: Implementar
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

        menu_printDiv();
        menu_printHeader("Selecionar operação");
        printf("Encomenda selecionada:");
        menu_printEncomendaBrief(encomendas.data[op]);
        int fnc = menu_selection(&(strvec){
            .data = (char*[]){
                "Marcar encomenda como expedida",
                "Marcar encomenda como cancelada",
                "Marcar encomenda como em entrega"
                "Marcar encomenda como entregue"
            },
            .size = 4
        });
        if(fnc == 0)      encomenda_ESTADO_EXPEDIDA   ( &(encomendas.data[op]) );
        else if(fnc == 1) encomenda_ESTADO_CANCELADA  ( &(encomendas.data[op]) );
        else if(fnc == 2) encomenda_ESTADO_EM_ENTREGA ( &(encomendas.data[op]) );
        else if(fnc == 3) encomenda_ESTADO_ENTREGUE   ( &(encomendas.data[op]) );
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

void interface_formalizar_encomenda() {
    menu_printDiv();
    menu_printHeader("Formalizar Encomenda");
    if(artigos.size == 0) {
        menu_printError("encomendas sem artigos são impossiveis");
        return;
    }

    menu_printInfo("utilizar a sua morada como morada de entrega?");
    morada dest;
    switch (menu_selection(&(strvec){
        .data = (char*[]){
            "Utilizar a minha morada como morada de entrega",
            "Definir outra morada como morada de entrega"
        },
        .size = 2
    })) {
        case -1: return;
        case  0: dest = morada_dup(utilizadores.data[utilizadorAtual].adereco); break;
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
    morada tmp = utilizadores.data[utilizadorAtual].adereco;
    utilizadores.data[utilizadorAtual].adereco = dest;
    encomenda e = encomenda_formalizar(artigos, tabelaPrecos, mult_CP, utilizadores.data[utilizadorAtual], org, (uint64_t)dist);
    utilizadores.data[utilizadorAtual].adereco = tmp;
    while (1) {
        menu_printInfo("definir encomenda como urgente?");
        switch (menu_selection(&(strvec){
            .data = (char*[]){
                "Definir encomenda como urgente",
                "Continuar"
            },
            .size = 2
        })) {
            case -1: goto END_OF_LOOP;
            case  0: encomenda_TIPO_URGENTE(&e); break;
            case  1: goto END_OF_LOOP;
        }
    }
    END_OF_LOOP:
    artigos = artigovec_new();
    encomendavec_push(&encomendas, e);
    menu_printInfo("encomenda formalizada com sucesso!");
}

int vecPrintArtigoPredicate (artigo item, int* userdata) {
    printf("   %*d   |   ", 8, ++(*userdata));
    menu_printArtigo(item);
    printf("\n");
    return 0;
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

    while (1) {
        menu_printInfo("numero de artigos atuais: %lu", artigos.size);
        menu_printHeader("Selecionar Artigo Para Editar");
        // Get option ---------------------------------
        int op = -2;
        int max = artigos.size;
        while(op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            int i = -1;
            artigovec_iterateFW(&artigos, (artigovec_predicate_t)&vecPrintArtigoPredicate, (void*)&i);
            printf("   %*d   |   Criar Novo Artigo\n", 8, ++i);
            printf("   %*d   |   Formalizar Encomenda\n", 8, ++i);
            while (!menu_readIntMinMax(-2, max+1, &op));
            menu_printDiv();
        }
        // Option was gotten -------------------------
        if(op == -1) return;
        if(op == max+1) {
            interface_formalizar_encomenda();
            return;
        }
        if(op == max) {
            artigovec_push(&artigos, newArtigo());
        }

        artigo* a = &(artigos.data[op]);

        printf("Introduzir Nome do Artigo (%s) $ ", a->nome);
        free(a->nome);
        a->nome = menu_readString(stdin);

        if(a->tratamentoEspecial) {
            printf("Introduzir Tratamento especial para o artigo Artigo (%s) $ ", a->tratamentoEspecial);
            free(a->tratamentoEspecial);
        }
        else printf("Introduzir Tratamento especial para o artigo Artigo $ ");
        a->tratamentoEspecial = menu_readString(stdin);
        if( *(a->tratamentoEspecial) == '\0') {
            // Sem tratamento especial
            free(a->tratamentoEspecial);
            a->tratamentoEspecial = NULL;
        }

        int tmp;
        while (1) {
            printf("Introduzir Peso do Artigo (em gramas) (%lu) $ ", a->peso_gramas);
            menu_readInt(&tmp);
            if(tmp < 0) menu_printError("artigo não pode ter peso negativo");
            else { a->peso_gramas = tmp; break; }
        }
        while (1) {
            printf("Introduzir Volume do Artigo (em centimetros cúbicos) (%lu) $ ", a->cmCubicos);
            menu_readInt(&tmp);
            if(tmp < 0) menu_printError("artigo não pode ter volume negativo");
            else { a->cmCubicos = tmp; break; }
        }
    }
}

void interface_consultar_tabela_de_precos() {
    menu_printDiv();
    menu_printHeader("Tabela de Preços");
    printf("ORIGEM DESTINO  0           1           2           3           4           5           6           7           8           9\n");
    for (int origem = 0; origem < 10; ++origem) {
        printf("  %d  |        ", origem);
        for (int destino = 0; destino < 10; ++destino) {
            printf("  %07.4f  |", mult_CP[origem][destino]);
        }
        printf("\n");
    }
    menu_printHeader("Tabela de Preços Base");
    printf("Preço regular:  %luc\n", tabelaPrecos.REGULAR );
    printf("Preço urgente:  %luc\n", tabelaPrecos.URGENTE );
    printf("Preço volumoso: %luc\n", tabelaPrecos.VOLUMOSO);
    printf("Preço fragil:   %luc\n", tabelaPrecos.FRAGIL  );
    printf("Preço pesado:   %luc\n", tabelaPrecos.PESADO  );
    printf("Preço por Km:   %luc\n", tabelaPrecos.POR_KM  );
}

void interface_editar_encomendas() {
    // TODO: implementar
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
            case  4: interface_consultar_tabela_de_precos(); break;
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
    free(u.nome);
    u.nome = menu_readString(stdin);

    char* stmp = NULL;
    do {
        printf("Introduzir NIF $ ");
        if(stmp) free(stmp);
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
    if(stmp) free(stmp);
    u.adereco.morada = menu_readString(stdin);

    do {
        printf("Introduzir Código Postal (XXXX-XXX) $ ");
        if(stmp) free(stmp);
        stmp = menu_readString(stdin);
        if (strlen(stmp) != 8) {
            menu_printError("tem que introduzir 8 characteres, %d introduzidos.", strlen(stmp));
        } else {
            u.adereco.codigoPostal[0] = stmp[0];
            u.adereco.codigoPostal[1] = stmp[1];
            u.adereco.codigoPostal[2] = stmp[2];
            u.adereco.codigoPostal[3] = stmp[3];
            u.adereco.codigoPostal[4] = stmp[5];
            u.adereco.codigoPostal[5] = stmp[6];
            u.adereco.codigoPostal[6] = stmp[7];
            if (!morada_eCPValido(u.adereco.codigoPostal)) {
                menu_printError("Código Postal introduzido é inválido.");
            } else break;
        }
    } while (1);

    do {
        printf("Introduzir numero do cartao de cidadão $ ");
        if(stmp) free(stmp);
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
            case -1: return;
        }
        printf("Introduzir NIF $ ");
        if(stmp) free(stmp);
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
    Gestão de encomendas -
    Deverá ser possível ao utilizador acrescentar uma nova encomenda a
    transportar, identificando a origem e destino.
    Deve ainda ser possível consultar o estado das encomendas, permitindo
    alterar/cancelar encomendas não expedidas.
    O Diretor atualiza o estado das encomendas sempre que estas forem expedidas.
*/

/*
    Gestão de preços -
    Deverá ser possível ao cliente consultar os preços tabelados para a
    realização de umtransporte. O diretor deverá ser capaz de criar/editar a
    tabela de preços.
    A alteração de preços não se deve refletir nas encomendas já inseridas.
*/

/*
    Geração de faturas -
    A aplicação deverá ser capaz de gerar/reimprimir a faturapara um cliente
    específico de um dado mês. A fatura deve ser gerada no ecrã e/ou escrita
    para ficheiro e deve conter a listagem de todas as encomendas expedidas
    nesse mês, com identificação da hora e do(s) artigo(s) transportados.
*/

/*
    Persistência de dados -
    A aplicação deverá permitir guardar/carregar dados em/de ficheiro,
    permitindo persisti-los ao longo de diferentes utilizações.
*/

void funcional_guardarDados() {
    // TODO: Implement
}

void funcional_carregarDados() {
    // TODO: Implement
}

/*
    Memória dinâmica -
    A aplicação deve, sempre que se justificar, utilizar memória dinâmica no
    armazenamento de informação.
*/

/*
    Cada grupo deve ainda propor e implementar, no mínimo, 5
    listagens/relatórios. Estas listagens/relatórios devem ser do interesse do
    cliente ou da empresa de transportes. O principal objetivo é avaliar a
    compreensão do problemabem como a capacidade do grupo de analisar os dados
    armazenados. Estas listagens devem ser claramente identificadas e descritas
    no relatório
*/

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