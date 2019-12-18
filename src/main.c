#include <stdint.h>
#include <string.h>
#include <memory.h>

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

artigovec     artigos;                          // Artigos da seção atual, que ainda não foram formalizados numa encomenda
encomendavec  encomendas;                       // Encomendas formalizadas pelo utilizador
utilizadorvec utilizadores;                     // Utilizadores existentes no registo, index 0 é diretor
size_t        utilizadorAtual = ~(size_t)0;     // Index do utilizador atual
float         multiplicadorPreco[10][10];       // Mapeia [Origem][Destino]
uint64_t      precoPorKm_cent;                  // Preço por kilometro em centimos
uint64_t     tabelaPrecoTipoTransporte_cent[5]; // Preço em centimos por cada tipo de transporte

/*
    Gestão de utilizadores -
    Deverá ser possível a um cliente criar, editar e remover o seu perfil de
    utilizador. Note que a remoção de um clientenão deve ter como resultado a
    remoção de toda a informação relativa a esse cliente,mas apenas a
    “marcação” como removido, não permitindo novo acesso, ou introdução de novas
    encomendas. O Diretor pode realizar qualquer uma das operações de
    manipulação de clientes inclusive reativar um cliente removido.
*/

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
    //TODO: Implementar
}

void interface_alterar_tabela_precos(){
    //TODO: Implementar
}

void interface_alterar_tabela_distancias(){
    //TODO: Implementar
}

void printUtilizador (utilizador u) {
    printf("%s (%c%c%c%c%c%c%c%c%c)", u.nome, u.NIF[0], u.NIF[1], u.NIF[2], u.NIF[3], u.NIF[4], u.NIF[5], u.NIF[6], u.NIF[7], u.NIF[8]);
}

int vecPrintUserPredicate (utilizador item, int* userdata) {
    printf("   %*d   |   ", 8, ++(*userdata));
    printUtilizador(item);
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
        while (1) {
            menu_printDiv();
            menu_printHeader("Selecionar operação");
            printf("Utilizador selecionado:");
            printUtilizador(utilizadores.data[op]);
            int fnc = menu_selection(&(strvec){
            .data = (char*[]){
                "Ativar utilizador", 
                "Desativar utilizador",
                "Editar utilizador"},
            .size = 3
            });
            if(fnc == -1) break;
            else if(fnc == 0) { 
                if(op==0) {menu_printError("Diretor está sempre ativo"); break; }
                utilizadores.data[op].tipo == UTILIZADOR_CLIENTE; 
                break; 
            }
            else if(fnc == 1) { 
                if(op==0) {menu_printError("Diretor está sempre ativo"); break; }
                utilizadores.data[op].tipo == UTILIZADOR_DESATIVADO; 
                break; 
            }
            else if(fnc == 2) { 
                if(op==0) interface_editar_diretor();
                else interface_editar_utilizador(op); 
                break; 
            }
        }
        
    }
    
}

void interface_editar_estados_encomendas(){
    //TODO: Implementar
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
                "Alterar estados das encomendas"
            },
            .size = 4
        })) {
            case -1: return;
            case  0: interface_alterar_tabela_precos();     break;
            case  1: interface_alterar_tabela_distancias(); break;
            case  2: interface_alterar_utilizadores();      break;
            case  3: interface_editar_estados_encomendas(); break;
        }
    }
    
}

void interface_utilizador() {
    // TODO: implementar
}

void interface_novoRegisto();
void funcional_carregarDados();

int utilizadorAtivadoNIFCompareVecPredicate(utilizador element, uint8_t* compare) {
    if(element.tipo == UTILIZADOR_DESATIVADO) return 0;
    return memcmp(element.NIF, compare, sizeof(uint8_t) * 9) == 0;
}

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
        if (utilizadorAtual != ~(size_t)0) utilizadorAtual = ~(size_t)0;
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
                interface_utilizador();
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