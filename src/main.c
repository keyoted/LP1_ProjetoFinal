/**
 * @file    main.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Ficheiro principal onde consta maior parte da lógica do programa.
 * @version 1
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 */

#ifdef DEBUG_BUILD
#    include <sys/stat.h>
#endif

#include <errno.h>

/**
 * @def U_INVAL
 *          Define um utilizador que é inválido.
 */
#define VEC_IMPLEMENTATION
#define U_INVAL ~((size_t) 0)
#include "menu.h"
#include "artigo.h"
#include "morada.h"
#include "utilities.h"
#include "encomenda.h"
#include "utilizador.h"

#ifndef artigovec_H
#    define artigovec_H
#    define VEC_TYPE artigo
#    define VEC_NAME artigovec
#    define VEC_DEALOC(X) freeArtigo(&X)
#    define VEC_WRITE(X, F) save_artigo(F, X)
#    define VEC_READ(X, F) load_artigo(F, X)
#    include "./vector.h"
#endif

#ifndef encomendavec_H
#    define encomendavec_H
#    define VEC_TYPE encomenda
#    define VEC_NAME encomendavec
#    define VEC_DEALOC(X) freeEncomenda(&X)
#    define VEC_WRITE(X, F) save_encomenda(F, X)
#    define VEC_READ(X, F) load_encomenda(F, X)
#    include "./vector.h"
#endif

#ifndef utilizadorvec_H
#    define utilizadorvec_H
#    define VEC_TYPE utilizador
#    define VEC_NAME utilizadorvec
#    define VEC_DEALOC(X) freeUtilizador(&X)
#    define VEC_WRITE(X, F) save_utilizador(F, X)
#    define VEC_READ(X, F) load_utilizador(F, X)
#    include "./vector.h"
#endif

artigovec     artigos;      ///< Artigos da seção atual, que ainda não foram formalizados numa encomenda.
encomendavec  encomendas;   ///< Encomendas formalizadas.
utilizadorvec utilizadores; ///< Utilizadores existentes no registo, index 0 é sempre o diretor.
precos_cent   tabelaPrecos; ///< Preço em centimos do transporte.
size_t        utilizadorAtual = U_INVAL; ///< Index do utilizador atual.

#include "outrasListagens.h"

/**
 * @brief          Pode ser utilizado como um iterador, imprime os utilizadores.
 * @param item     Utilizador a ser impresso.
 * @param userdata Deve ser inicializado como 0, no final irá conter o número de
 *                 utilizadores impresso.
 * @returns        0
 */
int printUserVP(utilizador* item, int* userdata) {
    printf("   %*d   |   ", 8, ++(*userdata));
    menu_printUtilizador(*item);
    printf("\n");
    return 0;
}

/**
 * @brief Imprime o recibe mensal para um certo utilizador.
 */
void funcional_recibo_mensal() {

    time_t     now   = time(NULL);
    struct tm* tmnow = localtime(&now);

    int mes = 0;
    int ano = 0;

    menu_printInfo("Inserir mês do recibo");
    menu_readIntMinMax(1, 12, &mes);

    menu_printInfo("Inserir ano do recibo");
    menu_readIntMinMax(1900, tmnow->tm_year + 1900, &ano);

    if (utilizadores.data[utilizadorAtual].tipo != UTILIZADOR_DIRETOR) {
        menu_printReciboMensal(utilizadorAtual, mes, ano, &encomendas, &utilizadores);
    } else {
        menu_printHeader("Selecionar Utilizador Para Quem Imprimir o Recibo");
        int op  = -2;
        int max = utilizadores.size - 1;
        while (op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            int i = -1;
            utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_pred_t) &printUserVP, (void*) &i);
            menu_readIntMinMax(-2, max, &op);
            menu_printDiv();
        }
        if (op == -1) return;
        menu_printReciboMensal(op, mes, ano, &encomendas, &utilizadores);
    }
}

/**
 * @brief Imprime os estados das encomendas.
 */
void funcional_consultar_estados_encomendas() {
    if (utilizadores.data[utilizadorAtual].tipo == UTILIZADOR_DIRETOR) {
        for (size_t i = 0; i < encomendas.size; ++i) {
            printf("   %*lu   |   ", 8, i);
            menu_printEncomendaBrief(&encomendas.data[i], &utilizadores);
            printf("\n");
        }
    } else {
        for (size_t i = 0; i < encomendas.size; ++i) {
            if (utilizadorAtual == encomendas.data[i].ID_cliente) {
                printf("   %*lu   |   ", 8, i);
                menu_printEncomendaBrief(&encomendas.data[i], &utilizadores);
                printf("\n");
            }
        }
    }
}

/**
 * @brief       Premite editar ou criar uma nova morada.
 * @param m     Morada onde que será editada.
 * @param isNew Deverá ser 1 se a morada não tiver sido inicializada, 0 caso
 *              contrário.
 */
void interface_editar_morada(morada* const m, int isNew) {

    if (isNew || (!m->morada)) {
        printf("Introduzir Morada");
        *m = newMorada();
    } else
        printf("Introduzir Morada (%s)", m->morada);
    menu_readNotNulStr(&m->morada);

    char* stmp = NULL;
    while (1) {
        if (isNew)
            printf("Introduzir Código Postal (XXXX-XXX)");
        else
            printf("Introduzir Código Postal (%.4s-%.3s)", m->codigoPostal, &m->codigoPostal[4]);
        menu_readNotNulStr(&stmp);

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
            } else
                break;
        }
    }
    free(stmp);
}

/**
 * @brief Pode ser utilizado como um iterador, premite encontrar um certo NIF no
 *        vetor de utilizadores, pesquisando só utilizadores ativos.
 * @param element Utilizador a ser comparado.
 * @param compare NIF a comparar.
 * @returns       1 se o NIF do 'element' for igual ao 'compare'.
 * @returns       0 caso contrário.
 */
int uAtivadoNIFcmpVP(utilizador* element, uint8_t compare[9]) {
    if (element->tipo == UTILIZADOR_DESATIVADO) return 0;
    return memcmp(element->NIF, compare, 9) == 0;
}

/**
 * @brief Pode ser utilizado como um iterador, premite encontrar um certo NIF no
 *        vetor de utilizadores.
 * @param element Utilizador a ser comparado.
 * @param compare NIF a comparar.
 * @returns       1 se o NIF do 'element' for igual ao 'compare'.
 * @returns       0 caso contrário.
 */
int uNIFcmpVP(utilizador* element, uint8_t compare[9]) { return memcmp(element->NIF, compare, 9) == 0; }

/**
 * @brief            Premite editar um utilizador.
 * @param ID         ID do utilizador a editar.
 * @param isDirector Se 1 premite editar diretores
 * @warning          Utilizadores[ID] é assumido existir.
 */
void funcional_editar_utilizador(size_t ID, int isDirector) {
    menu_printDiv();
    if (isDirector)
        menu_printHeader("Edição de Diretor");
    else
        menu_printHeader("Edição de Cliente");

    if (isDirector) { utilizadores.data[ID].tipo = UTILIZADOR_DIRETOR; }

    utilizador* u = &utilizadores.data[ID];

    printf("Introduzir Nome (%s)", protectStr(u->nome));
    menu_readNotNulStr(&u->nome);

    char* stmp = NULL;
    while (1) {
        printf("Introduzir NIF (%.9s)", u->NIF);
        menu_readNotNulStr(&stmp);
        if (strlen(stmp) != 9) {
            menu_printError("tem que introduzir 9 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eNIFValido((uint8_t*) stmp)) {
            menu_printError("NIF introduzido é inválido.");
        } else {
            // Verificar que o nif não é repetido
            size_t index = utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_pred_t) &uNIFcmpVP, stmp);
            if (index != INVAL_INDEX && index != ID) {
                // Nif existe e é diferente do utilizador atual
                menu_printError("NIF já existente no sistema.");
                continue;
            } else {
                memcpy(u->NIF, stmp, 9);
                break;
            }
        }
    }
    freeN(stmp);

    if (isDirector) {
        menu_printInfo("diretor editado com sucesso!");
        return;
    }

    interface_editar_morada(&u->endereco, 0);

    while (1) {
        printf("Introduzir numero do cartao de cidadão (%.12s)", u->CC);
        menu_readNotNulStr(&stmp);
        if (strlen(stmp) != 12) {
            menu_printError("tem que introduzir 12 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eCCValido((uint8_t*) stmp)) {
            menu_printError("CC introduzido é inválido.");
        } else {
            memcpy(&u->CC, stmp, sizeof(uint8_t) * 12);
            break;
        }
    }
    free(stmp);

    menu_printInfo("utilizador %s (%.9s) editado com sucesso!", protectStr(u->nome), u->NIF);
}

/**
 * @brief            Imprime informações sobre os preços atuais.
 * @param printDist  Se diferente de 0 imprime informação sobre a tabela de
 *                   distancias (multiplicadores de código postal.)
 * @param printTable Se diferente de 0 imprime informação sobre a tabela de
 *                   preços base.
 */
void funcional_consultar_tabela_de_precos(int printDist, int printTable) {
    if (printDist) {
        menu_printHeader("Tabela de Distancia");
        // TODO: partir por 80 colunas?
        printf("ORIGEM DESTINO   1           2           3           4 "
               "          5           6           7           8           9\n");
        for (int origem = 0; origem < 9; ++origem) {
            printf("  %d  |        ", origem + 1);
            for (int destino = 0; destino < 9; ++destino) {
                printf("  %7.4f  |", (double) tabelaPrecos.MULT_CP[origem][destino]);
            }
            printf("\n");
        }
    }
    if (printTable) {
        menu_printHeader("Tabela de Preços Base");
        printf("Preço regular:  %luc\n", tabelaPrecos.REGULAR);
        printf("Preço urgente:  %luc\n", tabelaPrecos.URGENTE);
        printf("Preço volumoso: %luc\n", tabelaPrecos.VOLUMOSO);
        printf("Preço fragil:   %luc\n", tabelaPrecos.FRAGIL);
        printf("Preço pesado:   %luc\n", tabelaPrecos.PESADO);
        printf("Preço por Km:   %luc\n", tabelaPrecos.POR_KM);
    }
}

/**
 * @brief Premite editar a tabela de preços.
 */
void interface_alterar_tabela_precos() {
    while (1) {
        funcional_consultar_tabela_de_precos(0, 1);
        menu_printDiv();
        menu_printHeader("Editar Preços Base");
        menu_printInfo("selecione o que editar:");
        int edit = menu_selection(&(strvec) {.data =
                                                 (char*[]) {
                                                     "Editar preço regular",  // 0
                                                     "Editar preço urgente",  // 1
                                                     "Editar preço volumoso", // 2
                                                     "Editar preço fragil",   // 3
                                                     "Editar preço pesado",   // 4
                                                     "Editar preço por Km"    // 5
                                                 },
                                             .size = 6});
        if (edit == -1) return;

        int novoPreco = 0;
        while (1) {
            printf("Introduza novo preço (em centimos)");
            menu_readInt(&novoPreco);
            if (novoPreco < 0) {
                menu_printError("Preço não pode ser negativo.");
            } else
                break;
        }

        switch (edit) {
            case 0: tabelaPrecos.REGULAR = novoPreco; return;
            case 1: tabelaPrecos.URGENTE = novoPreco; return;
            case 2: tabelaPrecos.VOLUMOSO = novoPreco; return;
            case 3: tabelaPrecos.FRAGIL = novoPreco; return;
            case 4: tabelaPrecos.PESADO = novoPreco; return;
            case 5: tabelaPrecos.POR_KM = novoPreco; return;
        }
    }
}

/**
 * @brief Premite editar a tabela das distâncias, a tabela dos multiplicadores
 *        de código postal.
 */
void interface_alterar_tabela_distancias() {
    funcional_consultar_tabela_de_precos(1, 0);
    menu_printInfo("selecione modo de edição:");
    int modo = menu_selection(&(strvec) {.data =
                                             (char*[]) {
                                                 "Editar preço de elemento",                               // 0
                                                 "Editar preço de linha (todos os destinos de uma origem)" // 1
                                             },
                                         .size = 2});
    if (modo == -1) return;

    int      origem  = 0;
    int      destino = 0;
    _Float32 novoVal = 0.0f;
    menu_printInfo("selecione linha de origem:");
    menu_readIntMinMax(1, 9, &origem);
    origem--;

    if (modo == 0) {
        menu_printInfo("selecione coluna de destino:");
        menu_readIntMinMax(1, 9, &destino);
        destino--;
        while (1) {
            printf("Introduza novo valor decimal para {origem:%d, destino:%d}", origem, destino);
            menu_read_Float32(&novoVal);
            if (novoVal < 0) {
                menu_printError("valor tem que ser maior que 0.");
            } else {
                tabelaPrecos.MULT_CP[origem][destino] = novoVal;
                break;
            }
        }
    } else {
        for (destino = 0; destino < 9; ++destino) {
            while (1) {
                printf("Introduza novo valor decimal para {origem:%d, destino:%d}", origem, destino + 1);
                menu_read_Float32(&novoVal);
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

/**
 * @brief Premite editar o estado do utilizador como cliente, desativado ou
 *        diretor. Premite também editar um utilizador.
 */
void interface_alterar_utilizadores() {
    while (1) {
        menu_printDiv();
        menu_printHeader("Selecionar Utilizador Para Editar");
        int op  = -2;
        int max = utilizadores.size - 1;
        while (op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            int i = -1;
            utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_pred_t) &printUserVP, (void*) &i);
            menu_readIntMinMax(-2, max, &op);
            menu_printDiv();
        }
        if (op == -1) return;

        menu_printDiv();
        menu_printHeader("Selecionar operação");
        printf("Utilizador selecionado:");
        menu_printUtilizador(utilizadores.data[op]);
        printf("\n");
        switch (menu_selection(&(strvec) {.data =
                                              (char*[]) {
                                                  "Marcar como cliente",    // 0
                                                  "Marcar como desativado", // 1
                                                  "Marcar como diretor",    // 2
                                                  "Editar utilizador"       // 3
                                              },
                                          .size = 4})) {
            case -1: return;
            case 0:
                if (op == 0) {
                    menu_printError("Utilizador 0 é sempre diretor.");
                } else
                    utilizadores.data[op].tipo = UTILIZADOR_CLIENTE;
                break;
            case 1:
                if (utilizadores.data[op].tipo == UTILIZADOR_DIRETOR) {
                    menu_printError("Um Diretor está sempre ativo.");
                } else
                    utilizadores.data[op].tipo = UTILIZADOR_DESATIVADO;
                break;
            case 2: utilizadores.data[op].tipo = UTILIZADOR_DIRETOR; break;
            case 3:
                switch (utilizadores.data[op].tipo) {
                    case UTILIZADOR_CLIENTE: funcional_editar_utilizador(op, 0); break;
                    case UTILIZADOR_DESATIVADO:
                        menu_printInfo("está a editar um utilizador desativado.");
                        funcional_editar_utilizador(op, 0);
                        break;
                    case UTILIZADOR_DIRETOR: funcional_editar_utilizador(op, 1); break;
                }
                break;
        }
    }
}

/**
 * @brief          Função auxiliar que funciona como um iterador. Imprime
 *                 encomendas.
 * @param item     Encomenda a imprimir.
 * @param userdata Deve ser inicializado como 0, no final irá conter o número de
 *                 encomendas impreço.
 * @returns        0
 */
int printEncomendaVP(encomenda* item, size_t* userdata) {
    printf("   %*lu   |   ", 8, (*userdata)++);
    menu_printEncomendaBrief(item, &utilizadores);
    printf("\n");
    return 0;
}

/**
 * @brief Menu que premite editar os estados das encomendas como expedida,
 *        cancelada, em entrega ou entregue.
 */
void interface_editar_estados_encomendas() {
    while (1) {
        menu_printDiv();
        menu_printHeader("Selecionar Encomenda Para Editar");
        int op  = -2;
        int max = encomendas.size - 1;
        while (op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            size_t i = 0;
            encomendavec_iterateFW(&encomendas, (encomendavec_pred_t) &printEncomendaVP, (void*) &i);
            menu_readIntMinMax(-2, max, &op);
            menu_printDiv();
        }
        if (op == -1) return;

        encomenda* e = &encomendas.data[op];
        menu_printDiv();
        while (1) {
            menu_printHeader("Selecionar operação:");
            switch (menu_selection(&(strvec) {.data =
                                                  (char*[]) {
                                                      "Marcar encomenda como cancelada",  // 0
                                                      "Marcar encomenda como em entrega", // 1
                                                      "Marcar encomenda como expedida",   // 2
                                                      "Marcar encomenda como entregue"    // 3
                                                  },
                                              .size = 4})) {
                case -1: return;
                case 0: encomenda_ESTADO_EXPEDIDA(e); break;
                case 1: encomenda_ESTADO_CANCELADA(e); break;
                case 2: encomenda_ESTADO_EM_ENTREGA(e); break;
                case 3: encomenda_ESTADO_ENTREGUE(e); break;
            }
        }
    }
}

/**
 * @brief Menu com todas as opções disponíveis ao diretor.
 */
void interface_diretor() {
    while (1) {
        menu_printDiv();
        menu_printHeader("Menu de Diretor");
        menu_printInfo("Bem vindo(a) %s!\n", protectStr(utilizadores.data[utilizadorAtual].nome));
        switch (menu_selection(&(strvec) {.data =
                                              (char*[]) {
                                                  "Editar dados de acesso",                  // 0
                                                  "Alterar tabela de preços base",           // 1
                                                  "Alterar tabela de distancias",            // 2
                                                  "Alterar estado dos utilizadores",         // 3
                                                  "Alterar estados das encomendas",          // 4
                                                  "Imprimir recibo de encomenda",            // 5
                                                  "Imprimir recibo mensal de um utilizador", // 6
                                                  "Consultar tabela de preços",              // 7
                                                  "Outras listagens"                         // 8
                                              },
                                          .size = 9})) {
            case -1: return;
            case 0: funcional_editar_utilizador(utilizadorAtual, 1); break;
            case 1: interface_alterar_tabela_precos(); break;
            case 2: interface_alterar_tabela_distancias(); break;
            case 3: interface_alterar_utilizadores(); break;
            case 4: interface_editar_estados_encomendas(); break;
            case 5: listagem_imprimir_recibo(); break;
            case 6: funcional_recibo_mensal(); break;
            case 7: funcional_consultar_tabela_de_precos(1, 1); break;
            case 8: interface_outrasListagens(); break;
        }
    }
}

/**
 * @brief Função auxiliar que premite desátivar o prefil do utilizador atual.
 */
void funcional_desativar_perfil_atual() {
    utilizadores.data[utilizadorAtual].tipo = UTILIZADOR_DESATIVADO;
    utilizadorAtual                         = U_INVAL;
    menu_printInfo("utilizador desativado com sucesso!");
}

// Artigos size is assumed to be diffrent than 0
/**
 * @brief         Adequire todas as informações necessarias para formalizar um
 *                artigo.
 * @param artigos Artigos que irão pretencer à encomenda formalizada.
 *                (tranfere o domínio)
 * @param e       Encomenda que será formalizada.
 * @returns       0 se não formalizado.
 * @returns       1 se formalizado.
 */
int funcional_formalizar_encomenda(artigovec* artigos, encomenda* e) {
    menu_printDiv();
    menu_printHeader("Formalizar Encomenda");

    menu_printInfo("utilizar a sua morada como morada de entrega?");
    morada dest;
    switch (menu_selection(&(strvec) {.data =
                                          (char*[]) {
                                              "Utilizar a minha morada como morada de entrega", // 0
                                              "Definir outra morada como morada de entrega"     // 1
                                          },
                                      .size = 2})) {
        case -1: return 0;
        case 0: dest = morada_dup(utilizadores.data[utilizadorAtual].endereco); break;
        case 1: interface_editar_morada(&dest, 1); break;
    }

    menu_printInfo("introduzir morada de origem.");
    morada org;
    interface_editar_morada(&org, 1);

    int dist = 0;
    while (1) {
        printf("Introduzir Distancia (em Km)");
        menu_readInt(&dist);
        if (dist < 0)
            menu_printError("distancia negativa não é possivél.");
        else
            break;
    }
    *e       = encomenda_formalizar(*artigos, tabelaPrecos, utilizadorAtual, org, dest, dist);
    *artigos = artigovec_new();
    while (1) {
        menu_printInfo("definir encomenda como urgente e/ou fragil?");
        switch (menu_selection(&(strvec) {.data =
                                              (char*[]) {
                                                  "Definir encomenda como urgente", // 0
                                                  "Definir encomenda como fragil",  // 1
                                                  "Continuar"                       // 2
                                              },
                                          .size = 3})) {
            case -1: goto END_OF_LOOP;
            case 0: encomenda_TIPO_URGENTE(e); break;
            case 1: encomenda_TIPO_FRAGIL_togle(e); break;
            case 2: goto END_OF_LOOP;
        }
    }
END_OF_LOOP:
    menu_printInfo("encomenda formalizada com sucesso!");
    return 1;
}

/**
 * @brief          Função auxiliar que funciona como um iterador. Imprime
 *                 artigos.
 * @param item     Artigo a imprimir.
 * @param userdata Deve ser inicializado como 0, no final irá conter o número de
 *                 artigos impreço.
 * @returns        0
 */
int printArtigoVP(artigo* item, size_t* userdata) {
    printf("   %*lu   |   ", 8, (*userdata)++);
    menu_printArtigo(item);
    printf("\n");
    return 0;
}

/**
 * @brief             Premite editar um artigo. (poderá ser dealocado)
 * @param a           Artigo sobre o qual operar.
 * @param isDeletable Se não zero, premite que o artigo seja eleminado.
 * @returns           0 se o artigo foi eleminado.
 * @returns           1 se o artigo foi editado.
 */
int funcional_editar_artigo(artigo* a, int isNew) {
    if (!isNew) {
        switch (menu_selection(&(strvec) {.data =
                                              (char*[]) {
                                                  "Apagar artigo", // 0
                                                  "Editar Artigo"  // 1
                                              },
                                          .size = 2})) {
            case -1: return 1;
            case 0: freeArtigo(a); return 0;
            case 1: break;
        }
    }

    printf("Introduzir Nome do Artigo (%s)", protectStr(a->nome));
    menu_readNotNulStr(&a->nome);

    if (a->tratamentoEspecial) {
        printf("Introduzir Tratamento especial para o artigo Artigo (%s)", a->tratamentoEspecial);
        freeN(a->tratamentoEspecial);
    } else {
        printf("Introduzir Tratamento especial para o artigo Artigo");
    }
    a->tratamentoEspecial = menu_readString();

    int tmp = 0;
    while (1) {
        printf("Introduzir Peso do Artigo (em gramas) (%lu)", a->peso_gramas);
        menu_readInt(&tmp);
        if (tmp < 0)
            menu_printError("artigo não pode ter peso negativo.");
        else {
            a->peso_gramas = tmp;
            break;
        }
    }
    while (1) {
        printf("Introduzir Volume do Artigo (em centimetros cúbicos) (%lu)", a->cmCubicos);
        menu_readInt(&tmp);
        if (tmp < 0)
            menu_printError("artigo não pode ter volume negativo.");
        else {
            a->cmCubicos = tmp;
            break;
        }
    }
    return 1;
}

/**
 * @brief    Função que premite editar, adicionar ou remover artigos de um vetor
 *           de artigos.
 * @param ar Vetor no qual operar.
 */
void funcional_editar_artigos(artigovec* ar) {
    while (1) {
        menu_printInfo("numero de artigos atuais: %lu.", ar->size);
        menu_printHeader("Selecionar Artigo Para Editar:");
        int    op  = -2;
        size_t max = 0;
        while (op == -2) {
            printf("   Opção      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            artigovec_iterateFW(ar, (artigovec_pred_t) &printArtigoVP, (void*) &max);
            printf("   %*lu   |   Criar Novo Artigo\n", 8, max);
            menu_readIntMinMax(-2, max, &op);
            menu_printDiv();
        }
        // Option was gotten -------------------------
        if (op == -1) return;
        if (op == (int) max) {
            protectFcnCall(artigovec_push(ar, newArtigo()), "funcional_editar_artigos - artigovec_push falhou.");
        }
        if (!funcional_editar_artigo(&ar->data[op], (op == (int) max))) {
            artigovec_moveBelow(ar, op);
            menu_printInfo("Artigo removido.");
        }
    }
}

/**
 * @brief Menu que premite criar uma nova encomenda.
 */
void interface_criar_encomenda() {
    menu_printDiv();
    menu_printHeader("Criar Nova Encomenda");
    if (artigos.size != 0) {
        menu_printInfo("encomenda por formalizar encontrada, o que deseja fazer?");
        switch (menu_selection(&(strvec) {.data =
                                              (char*[]) {
                                                  "Descartar encomenda anterior", // 0
                                                  "Continuar encomenda anterior"  // 1
                                              },
                                          .size = 2})) {
            case -1: return;
            case 0:
                artigovec_free(&artigos);
                artigos = artigovec_new();
                break;
            case 1: break;
        }
    }
    funcional_editar_artigos(&artigos);
    menu_printInfo("a formalizar encomenda...");
    if (artigos.size == 0) {
        menu_printError("impossivel formalizar encomenda sem artigos!");
        return;
    }

    encomenda e;
    if (!funcional_formalizar_encomenda(&artigos, &e)) {
        menu_printInfo("encomenda não formalizada.");
    } else {
        protectFcnCall(encomendavec_push(&encomendas, e), "interface_criar_encomenda - encomendavec_push falhou.");
        menu_printInfo("encomenda adicionada com sucesso!");
    }
}

/**
 * @brief Menu que premite editar encomendas.
 */
void interface_editar_encomendas() {
    menu_printDiv();
    menu_printHeader("Selecione Encomenda Para Editar");
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
    encomenda* e = &encomendas.data[op];
    menu_printEncomendaDetail(e, &utilizadores);

    if (e->tipoEstado & ENCOMENDA_ESTADO_EM_ENTREGA) {
        // Encomenda está por entregar
        while (1) {
            menu_printHeader("Encomenda Por Entregar");
            switch (menu_selection(&(strvec) {.data =
                                                  (char*[]) {
                                                      "Editar artigos da encomenda",    // 0
                                                      "Cancelar encomenda",             // 1
                                                      "Definir encomenda como urgente", // 2
                                                      "Definir encomenda como fragil",  // 3
                                                      "Alterar morada de origem",       // 4
                                                      "Alterar morada de destino",      // 5
                                                      "Alterar a distância"             // 6
                                                  },
                                              .size = 7})) {
                case -1: return;
                case 0: {
                    funcional_editar_artigos(&e->artigos);
                    menu_printInfo("a formalizar encomenda...");
                    e->tipoEstado = ENCOMENDA_ESTADO_EM_ENTREGA;
                    if (e->artigos.size == 0) {
                        menu_printError("impossivel formalizar encomenda sem "
                                        "artigos, a cancelar encomenda!");
                        e->tipoEstado = ENCOMENDA_ESTADO_CANCELADA;
                    }
                    encomenda_TIPO_VOLUMOSO(e);
                    encomenda_TIPO_FRAGIL(e);
                    encomenda_TIPO_PESADO(e);
                    return;
                }
                case 1: encomenda_ESTADO_CANCELADA(e); return;
                case 2: encomenda_TIPO_URGENTE(e); break;
                case 3: encomenda_TIPO_FRAGIL_togle(e); break;
                case 4: interface_editar_morada(&e->origem, 0); break;
                case 5: interface_editar_morada(&e->destino, 0); break;
                case 6: {
                    while (1) {
                        printf("Introduzir Distancia (em Km)");
                        int d = 0;
                        menu_readInt(&d);
                        if (d < 0)
                            menu_printError("distancia negativa não é possivél.");
                        else {
                            e->distancia_km = d;
                            break;
                        }
                    }
                } break;
            }
        }
    } else if (e->tipoEstado & ENCOMENDA_ESTADO_CANCELADA) {
        menu_printHeader("Encomenda Cancelada");
        switch (menu_selection(&(strvec) {.data =
                                              (char*[]) {
                                                  "Reativar encomenda" // 0
                                              },
                                          .size = 1})) {
            case -1: return;
            case 0: {
                if (e->artigos.size == 0) {
                    menu_printError("impossivel reativar encomenda sem artigos!");
                } else {
                    encomenda_ESTADO_EM_ENTREGA(e);
                }
                return;
            }
        }
    } else if (e->tipoEstado & ENCOMENDA_ESTADO_ENTREGUE) {
        menu_printError("a encomenda foi entregue, não pode ser editada.");
    } else if (e->tipoEstado & ENCOMENDA_ESTADO_EXPEDIDA) {
        menu_printError("a encomenda foi expedida, não pode ser editada.");
    }
}

/**
 * @brief Menu das ações que um utilizador pode realizar.
 */
void interface_cliente() {
    while (1) {
        if (utilizadorAtual == U_INVAL) return;
        menu_printDiv();
        menu_printHeader("Menu de Cliente");
        menu_printInfo("Bem vindo(a) %s!\n", protectStr(utilizadores.data[utilizadorAtual].nome));
        strvec vetorOp = (strvec) {.data =
                                       (char*[]) {
                                           "Editar perfil",                  // 0
                                           "Remover perfil",                 // 1
                                           "Criar nova encomenda",           // 2
                                           "Consultar estado de encomendas", // 3
                                           "Consultar tabela de preços",     // 4
                                           "Editar encomendas",              // 5
                                           "Imprimir recibo de encomenda",   // 6
                                           "Imprimir recibo mensal"          // 7
                                       },
                                   .size = 8};
        switch (menu_selection(&vetorOp)) {
            case -1: return;
            case 0: funcional_editar_utilizador(utilizadorAtual, 0); break;
            case 1: funcional_desativar_perfil_atual(); break;
            case 2: interface_criar_encomenda(); break;
            case 3:
                menu_printDiv();
                menu_printHeader("Encomendas");
                funcional_consultar_estados_encomendas();
                break;
            case 4: funcional_consultar_tabela_de_precos(1, 1); break;
            case 5: interface_editar_encomendas(); break;
            case 6: listagem_imprimir_recibo(); break;
            case 7: funcional_recibo_mensal(); break;
        }
    }
}

/**
 * @brief Cria um novo estado e um diretor com o ID de 0.
 */
void interface_novoRegisto() {
    menu_printDiv();
    menu_printHeader("Novo Registo");
    if (utilizadores.size != 0) {
        menu_printInfo("ainda tem um registo aberto!");
        menu_printInfo("tem a certeza que quer descartar os dados do registo?");
        switch (menu_selection(&(strvec) {.data =
                                              (char*[]) {
                                                  "Descartar registo" // 0
                                              },
                                          .size = 1})) {
            case -1: return;
            case 0: break;
        }
    }

    artigovec_free(&artigos);
    encomendavec_free(&encomendas);
    utilizadorvec_free(&utilizadores);
    artigos      = artigovec_new();
    encomendas   = encomendavec_new();
    utilizadores = utilizadorvec_new();

    menu_printHeader("Registar Novo Diretor");

    protectFcnCall(utilizadorvec_push(&utilizadores, newUtilizador()),
                   "interface_novoRegisto - utilizadorvec_push falhou.");
    funcional_editar_utilizador(0, 1);
    menu_printInfo("diretor criado com sucesso!");
}

/**
 * @brief   Função auxiliar para limpar o estado após 'funcional_carregarDados'
 *          entrar em erro.
 * @param f Ficheiro para encerrar.
 */
void funcional_carregarDados_err(FILE* f) {
    fclose(f);
    artigovec_free(&artigos);
    encomendavec_free(&encomendas);
    utilizadorvec_free(&utilizadores);
    artigos      = artigovec_new();
    encomendas   = encomendavec_new();
    utilizadores = utilizadorvec_new();
}

/**
 * @brief Carrega o estado do programa de ficheiro.
 */
void funcional_carregarDados() {
    menu_printInfo("tem a certeza que quer CARREGAR os dados de ficheiro?");
    switch (menu_selection(&(strvec) {.data =
                                          (char*[]) {
                                              "Descartar registo e carregar dados de ficheiro" // 0
                                          },
                                      .size = 1})) {
        case -1: return;
        case 0: break;
    }
    artigovec_free(&artigos);
    encomendavec_free(&encomendas);
    utilizadorvec_free(&utilizadores);
    artigos      = artigovec_new();
    encomendas   = encomendavec_new();
    utilizadores = utilizadorvec_new();

    FILE* f = fopen("data.bin", "rb");
    if (!f) {
        menu_printError("impossivel abrir ficheiro :%d!", errno);
        return;
    }
    if (!load_precos(f, &tabelaPrecos)) {
        menu_printError("impossivel carregar tabela de preços :%d!", errno);
        funcional_carregarDados_err(f);
        return;
    }
    // Carregar utilizadores
    if (!utilizadorvec_read(&utilizadores, f)) {
        menu_printError("impossivel carregar utilizadores %d!", errno);
        funcional_carregarDados_err(f);
        return;
    }
    // Carregar encomendas
    if (!encomendavec_read(&encomendas, f)) {
        menu_printError("impossivel carregar encomendas %d!", errno);
        funcional_carregarDados_err(f);
        return;
    }
    menu_printInfo("Dados carregados com sucesso!");
    fclose(f);
}

/**
 * @brief Regista um novo utilizador como cliente.
 */
void interface_registoUtilizador() {
    menu_printDiv();
    menu_printHeader("Registo de Novo Utilizador");

    while (utilizadores.size == 0) {
        menu_printInfo("ainda não tem um registo aberto, quer criar um novo registo?");
        switch (menu_selection(&(strvec) {.data =
                                              (char*[]) {
                                                  "Criar um novo registo",       // 0
                                                  "Carregar registo de ficheiro" // 1
                                              },
                                          .size = 2})) {
            case -1: return;
            case 0: interface_novoRegisto(); return;
            case 1: funcional_carregarDados(); return;
        }
    }

    menu_printHeader("Registar Novo Utilizador");
    protectFcnCall(utilizadorvec_push(&utilizadores, newUtilizador()),
                   "interface_registoUtilizador - utilizadorvec_push falhou.");
    funcional_editar_utilizador(utilizadores.size - 1, 0);
    utilizadores.data[utilizadores.size - 1].tipo = UTILIZADOR_CLIENTE;
    menu_printInfo("utilizador %s (%.9s) adicionado com sucesso!",
                   protectStr(utilizadores.data[utilizadores.size - 1].nome),
                   utilizadores.data[utilizadores.size - 1].NIF);
}

/**
 * @brief Premite aos utilizadores efetuarem login, gravando o ID do utilizador
 *        cujo login foi efutuado em 'utilizadorAtual'.
 */
void interface_login() {
    char* stmp = NULL;
    while (1) {
        utilizadorAtual = U_INVAL;
        if (artigos.size != 0) {
            menu_printInfo("encomenda por formalizar foi eleminada.");
            artigovec_free(&artigos);
            artigos = artigovec_new();
        }
        menu_printDiv();
        menu_printHeader("Efetuar Login");
        printf("Introduzir NIF");
        menu_readNotNulStr(&stmp);
        if (strlen(stmp) != 9) {
            menu_printError("tem que introduzir 9 characteres, %d introduzidos.", strlen(stmp));
        } else if (!utilizador_eNIFValido((uint8_t*) stmp)) {
            menu_printError("NIF introduzido é inválido.");
        } else {
            size_t index = utilizadorvec_iterateFW(&utilizadores, (utilizadorvec_pred_t) &uAtivadoNIFcmpVP, stmp);
            if (index == U_INVAL) {
                menu_printError("NIF não reconhecido como um utilizador.");
            } else {
                switch (utilizadores.data[index].tipo) {
                    case UTILIZADOR_DESATIVADO:
                        menu_printError("o seu prefil está desativado, "
                                        "contacte um diretor para o ativar...");
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
                    default: menu_printError("utilizador invalido!"); break;
                }
            }
        }
        menu_printDiv();
        menu_printHeader("Efetuar Login ?");
        switch (menu_selection(&(strvec) {.data =
                                              (char*[]) {
                                                  "Efetuar login" // 0
                                              },
                                          .size = 1})) {
            case -1: freeN(stmp); return;
        }
    }
}

/**
 * @brief Guarda o estado atual do programa.
 */
void funcional_guardarDados() {
    menu_printInfo("tem a certeza que quer GRAVAR os dados em ficheiro?");
    switch (menu_selection(&(strvec) {.data =
                                          (char*[]) {
                                              "Gravar dados e substituir o ficheiro anterior" // 0
                                          },
                                      .size = 1})) {
        case -1: return;
        case 0: break;
    }
    FILE* f = fopen("data.bin", "wb");
    if (!f) {
        menu_printError("impossivel abrir ficheiro :%d!", errno);
        return;
    }
    if (!save_precos(f, &tabelaPrecos)) {
        menu_printError("impossivel guardar tabela de preços :%d!", errno);
        fclose(f);
        return;
    }
    // Gravar utilizadores
    if (!utilizadorvec_write(&utilizadores, f)) {
        menu_printError("impossivel guardar utilizadores :%d!", errno);
        fclose(f);
        return;
    }
    // Gravar encomendas
    if (!encomendavec_write(&encomendas, f)) {
        menu_printError("impossivel guardar encomendas :%d!", errno);
        fclose(f);
        return;
    }
    menu_printInfo("Dados guardados com sucesso!");
    fclose(f);
}

/**
 * @brief Apresenta um menu inicial ao utilizador.
 */
void interface_inicio() {
    while (1) {
        menu_printDiv();
        menu_printHeader("Início");
        switch (menu_selection(&(strvec) {.size = 5,
                                          .data = (char*[]) {
                                              "Log In",              // 0
                                              "Registar utilizador", // 1
                                              "Carregar dados",      // 2
                                              "Guardar dados",       // 3
                                              "Novo registo"         // 4
                                          }})) {
            case -1: return;
            case 0: interface_login(); break;
            case 1: interface_registoUtilizador(); break;
            case 2: funcional_carregarDados(); break;
            case 3: funcional_guardarDados(); break;
            case 4: interface_novoRegisto(); break;
        }
    }
}

/**
 * @brief   Ponto de entrada do programa, inicia as variáveis globais, chama
 *          interface_inicio e desaloca as globais no final.
 * @returns 0
 */
int main() {
#ifdef DEBUG_BUILD
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("DEBUG BULD\n");
    printf("COMPILADO EM: " __DATE__ " - " __TIME__ "\n");
    printf("COMPILADOR: "
#    if defined(_MSC_VER)
           "VISUAL STUDIO"
           " \nVERÇÂO DO COMPILADOR: " MACRO_QUOTE(_MSC_VER)
#    elif defined(__clang__)
           "CLANG"
           " \nVERÇÂO DO COMPILADOR: " MACRO_QUOTE(__clang_major__) "." MACRO_QUOTE(__clang_minor__) "." MACRO_QUOTE(
               __clang_patchlevel__)
#    elif defined(__MINGW32__)
           "MINGW"
           " \nVERÇÂO DO COMPILADOR: " MACRO_QUOTE(__MINGW32_MAJOR_VERSION) "." MACRO_QUOTE(__MINGW32_MINOR_VERSION)
#    elif defined(__TINYC__)
           "TINY C"
#    elif defined(__llvm__)
           "DESCONHECIDO - LLVM BACKEND"
#    elif defined(__GNUC__) && !defined(__INTEL_COMPILER)
           "GNU C COMPILER"
           " \nVERÇÂO DO COMPILADOR: " MACRO_QUOTE(__GNUC__) "." MACRO_QUOTE(__GNUC_MINOR__)
#    else
           "DESCONHECIDO"
#    endif
               "\n");
#endif

    menu_printDiv();
    menu_printHeader("A Iniciar");
    artigos      = artigovec_new();
    encomendas   = encomendavec_new();
    utilizadores = utilizadorvec_new();

    interface_inicio();
    menu_printHeader("A Terminar");

    artigovec_free(&artigos);
    encomendavec_free(&encomendas);
    utilizadorvec_free(&utilizadores);
    menu_printDiv();

    return 0;
}