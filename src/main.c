#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

typedef struct {
    char* nome;
    char* tratamentoEspecial;
    uint16_t gramas;
    uint16_t milimetrosCubicos
} artigo;

int ePesado(artigo* a) {
    return a->gramas > 20000;
}

int eVolumoso(artigo* a) {
    return a->milimetrosCubicos > (500 * 500 * 500);
}

artigo* newArtigo() {
    return calloc(1, sizeof(artigo));
}

void freeArtigo(artigo* a) {
    free(a->nome);
    free(a->tratamentoEspecial);
    free(a);
}

/*
    Encomenda -
    Constitui um conjunto de artigos que serão transportados em conjunto desde
    uma determinada origem até ao seu destino. A encomenda deverá ter associado
    o tipo de entrega, ou seja, identificar se é consideradacomo urgente ou
    serviço regular;
*/

typedef struct {
    char* morada;
    uint8_t codigoPostal[7];
} morada;

morada* newMorada() {
    return calloc(1, sizeof(morada));
}

void freeMorada(morada* m) {
    free(m->morada);
    free(m);
}

#define URGENTE 1
#define REGULAR 0

#define VEC_TYPE artigo*
#define VEC_NAME artigoPvec
#define VEC_DEALOC(X) freeArtigo(X);
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC

typedef struct {
    artigoPvec artigos;
    morada* origem;
    morada* destino;
    uint8_t urgencia;
    uint64_t precoCentimos;
} encomenda;

encomenda* newEncomenda() {
    encomenda* e = malloc(sizeof(encomenda));
    e->artigos = artigoPvec_new();
    e->origem = newMorada();
    e->destino = newMorada();
    e->urgencia = REGULAR;
    e->precoCentimos = 0;
    return e;
}

void freeEncomenda(encomenda* e) {
    artigoPvec_free(&e->artigos);
    freeMorada(e->origem);
    freeMorada(e->destino);
    free(e);
}

/*
    Custo de transporte -
    Representao valor final que o cliente terá de pagar à empresa transportadora
    de acordo com o tipo de encomenda, a distância percorrida (em quilómetros),
    e com a região de origem e de destino;
*/

// ??????????????

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


#define CLIENTE 0
#define DIRETOR 1

typedef struct {
    char* nome;
    uint8_t NIF[9];
    uint8_t CC[12];
    morada* adereco;
    uint8_t tipo;
} utilizador;

int eCCValido (uint8_t cc[12]) {
    int i = 0;
    // Assegurar 9 digitos
    for(i; i < 9; ++i)
        if(!isdigit(cc[i]))
            return 0;
    // Assegurar 2 digitos alfanuméricos
    for(i; i < 11; ++i) {
        if(isalnum(cc[i])) {
            cc[i] = (char)toupper(cc[i]);
        } else return 0;
    }
    // Asegurar ultimo digito
    if(! isdigit(cc[11])) return 0;

    return 1;
}

utilizador* novoUtilizador() {
    utilizador* u = calloc(1, sizeof(utilizador));
    u->adereco = newMorada();
    u->tipo = CLIENTE;
}

void freeUtilizador(utilizador* u) {
    free(u->nome);
    freeMorada(u->adereco);
    free(u);
}

/*
    Funcionalidades comuns -
    Esta secção descreve o  conjunto de funcionalidades a implementar que são
    comuns a todos os grupos.
    Pretende-se que modele as estruturas de dados e desenvolva as funções
    necessárias para implementarasseguintes funcionalidades:
*/

/*
    Gestão de utilizadores -
    Deverá ser possível a um cliente criar, edita reremover o seu perfil de
    utilizador. Note que a remoção de um clientenão deve ter como resultado a
    remoção de toda a informação relativa a esse cliente,mas apenas a
    “marcação” como removido, não permitindo novo acesso, ou introdução de novas
    encomendas. O Diretor pode realizar qualquer uma das operações de
    manipulação de clientes inclusive reativar um cliente removido.
*/

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

int main() {
    return 0;
}