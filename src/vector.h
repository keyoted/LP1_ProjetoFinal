#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

/**
 * @file    vector.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Protótipo para um vetor, um objeto que comtém um numero variável de
 *          objetos e que é capaz de alocar memoria dinâmicamente e de dealocar
 *          memória quando necessáro. Este ficheiro contém apenas um 'protótipo'
 *          que é compilado para um objeto real quando os macros VEC_TYPE;
 *          VEC_NAME e VEC_DEALOC(X) são definidos antes de
 *          incluir o ficheiro.
 * @version 0.1
 * @date    2019-12-04
 *
 * @copyright Copyright (c) 2019
 * @section example_sec Exemplo
 * @code{c}
 * #define  VEC_TYPE             char*
 * #define  VEC_NAME             strvec
 * #define  VEC_DEALOC(X)        free(X)
 * #include "./vector.h"
 * #undef   VEC_TYPE
 * #undef   VEC_NAME
 * #undef   VEC_DEALOC
 *
 * #define  VEC_TYPE             int
 * #define  VEC_NAME             intvec
 * #define  VEC_DEALOC(x)
 * #include "./vector.h"
 * #undef   VEC_TYPE
 * #undef   VEC_NAME
 * #undef   VEC_DEALOC
 * @endcode
 */

/**
 * @def VEC_TYPE
 *                  O tipo de dados guardado no vetor.
 * @def VEC_NAME
 *                  Nome da nova struct, todas as funções têm este nome como
 *                  prefixo.
 * @def VEC_DEALOC(X)
 *                  Maneira de dealocar VEC_TYPE, se necessário.
 * @def PASTER(X, Y)
 *                  Macro auxiliar para juntar VEC_NAME e Y.
 * @def EVAL(X, Y)
 *                  Macro auxiliar para avaliar VEC_NAME de modo a poder
 *                  juntá-lo com o nome da função.
 * @def VEC_FUN(X)
 *                  Macro responsavél por adicionar VEC_NAME antes do nome de
 *                  cada função.
 */

#ifndef VEC_TYPE
#define VEC_TYPE int
#endif

#ifndef VEC_NAME
#define VEC_NAME vector
#endif

#ifndef VEC_DEALOC
#define VEC_DEALOC(x)
#endif

#define PASTER(X, Y) X##Y
#define EVAL(X, Y) PASTER(X, Y)
#define VEC_FUN(X) EVAL(VEC_NAME, X)

/**
 * @brief           Struct com o nome VEC_NAME que contém o tipo de dados
 *                  VEC_TYPE e informação sobre o numero de objetos guardado,
 *                  pode guardar [0, ~((size_t)0)[ elemntos.
 */
typedef struct {
    size_t alocated;    ///< Tamanho alocado de objetos.
    size_t size;        ///< Tamanho de objetos que foi populado.
    VEC_TYPE* data;     ///< Começa em data[0] e acaba em data[size-1].
} VEC_NAME;

/**
 * @brief           Aumenta v, se necessário e possivél, para que se possa
 *                  adicionar mais objetos no final.
 * @param v         Vetor a que queremos adicionar espaço.
 * @returns         0 se tentou mas não conseguiu alocar memória.
 * @returns         1 se alocou memória.
 * @returns         2 se não o necessiou de alocar memoria.
 */
int VEC_FUN(_addCell) (VEC_NAME* const v) {
    if(v->alocated == 0) {
        // Vetor vazio
        v->data = malloc(sizeof(VEC_TYPE) * 8);
        if(v->data == NULL) return 0;
        v->alocated = 8;
        return 1;
    } else if(v->size == v->alocated) {
        // O vetor está cheio e necessita de ser redimensionado
        void* newData = realloc(v->data, sizeof(VEC_TYPE)*(v->alocated*2));
        if(newData == NULL) {
            // Foi impossivel realocar memoria sufuciente
            // Tentar realocar apenas mais um espaço
            newData = realloc(v->data, sizeof(VEC_TYPE)*(v->alocated+1));
            if(newData == NULL) {
                // Impossivel alocar mais memoria
                return 0;
            } else {
                // Memoria alocada, fazer o update do vetor
                v->alocated += 1;
                v->data = newData;
                return 1;
            }
        } else {
            // Memoria alocada, fazer o update do vetor
            v->alocated *= 2;
            v->data = newData;
            return 1;
        }
    }else {
        // Vetor ainda tem espaço livre
        return 2;
    }
}

/**
 * @brief           Constrotor do vetor.
 * @details         Tenta constuir um vetor vazio com 8 céluals de memória
 *                  alocadas.
 * @return          Um vetor vazio.
 */
VEC_NAME VEC_FUN(_new)() {
    VEC_NAME v;
    v.size = 0;
    v.alocated = 0;
    v.data = NULL;
    VEC_FUN(_addCell)(&v);
    return v;
}

/**
 * @brief           Adiciona um objeto no final do vetor.
 * @param v         Ponteiro para o vetor sob o qual operar.
 * @param newObj    Objeto para adicionar ao vetor.
 * @returns         0 se não conseguiu inserir o objeto.
 * @returns         1 se conseguiu inserir o objeto.
 */
int VEC_FUN(_push)(VEC_NAME* const v, VEC_TYPE const newObj) {
    if(!VEC_FUN(_addCell)(v)) return 0;
    // Aqui está garantido que temos pelo menos mais um espaço alocado no vetor
    v->data[v->size] = newObj;
    ++(v->size);
    return 1;
}

/**
 * @brief           Move todos os elementos acima de 'i' um espaço para baixo,
 *                  escrevendo sobre 'i'.
 * @details         Todos os elementos com indexes maiores que 'i' são movidos
 *                  um index para baixo, escrevendo por cima do elemento no
 *                  index 'i' e diminuindo o tamanho de 'v' por uma unidade.
 * @param v         Ponteiro do vetor sob o qual operar.
 * @param i         Index do objeto que será escrito por cima.
 * @warning         O objecto no index 'i' não é dealocado por esta função
 */
void VEC_FUN(_moveBelow)(VEC_NAME* const v, const size_t i) {
    memmove(&(v->data[i]), &(v->data[i+1]), (v->size-i-1)*sizeof(VEC_TYPE));
    --(v->size);
}

/**
 * @brief           Move todos os elementos acima de i e i para cima, criando um
 *                  espaço livre no index i.
 * @details         Todos os elementos com index igual ou superior a 'i' serão
 *                  movidos um index acima, aumentando o tamanho de 'v' por uma
 *                  unidade e deixando um espaço livre no index 'i'. O vetor
 *                  pode ter que ser realocado durante este processo.
 * @param v         Ponteiro do vetor sob o qual operar.
 * @param i         Index do espaço livre após chamar a função.
 * @returns         0 se não conseguiu realizar a operação.
 * @returns         1 se conseguiu realizar a operação.
 * @warning         Após chamar esta função, o objeto no index i pode não ser
 *                  válido.
 */
int VEC_FUN(_moveAbove)(VEC_NAME* const v, const size_t i) {
    if(!VEC_FUN(_addCell)(v)) return 0;
    memmove(&(v->data[i+1]), &(v->data[i]), (v->size-i)*sizeof(VEC_TYPE));
    ++(v->size);
    return 1;
}

// Adiciona newElement na posição position do vetor
// Retorna 1 em sucesso, 0 de outro modo
/**
 * @brief           Adiciona um objeto no vetor.
 * @param v         Ponteiro para o vetor sob o qual operar.
 * @param newObj    Objeto para adicionar ao vetor.
 * @param position  Index do vetor onde adicionar o objeto.
 * @note            'position' terá que ser menor ou igual ao tamanho do vetor.
 * @returns         0 se não conseguiu inserir o objeto.
 * @returns         1 se conseguiu inserir o objeto.
 */
int VEC_FUN(_pushAt)
    (VEC_NAME* const v, VEC_TYPE const newObj, const size_t position) {
    if(!VEC_FUN(_moveAbove)(v, position)) return 0;
    v->data[position] = newObj;
    return 1;
}

/**
 * @brief           Retorna e remove o último objeto do vetor.
 * @details         Retorna o o último objeto do vetor, removendo-o, mas sem o
 *                  dealocar.
 * @param v         Vetor sob o qual operar.
 * @returns         O o último objeto do vetor.
 * @warning         O o último objeto do vetor é removido e não é dealocado,
 *                  terá que ser dealocado posteriormente.
 */
VEC_TYPE VEC_FUN(_pop)(VEC_NAME* const v) {
    VEC_TYPE toReturn = v->data[v->size-1];
    --(v->size);
    return toReturn;
}

/**
 * @brief           Retorna e remove um objeto do vetor.
 * @details         Retorna o objeto com o index 'position' do vetor,
 *                  removendo-o, mas sem o dealocar.
 * @param v         Vetor sob o qual operar.
 * @param position  Index do objeto que será removido do vetor.
 * @returns         O objeto com o index 'position'
 * @warning         O objeto com o index 'position' é removido do vetor e não é
 *                  dealocado, terá que ser dealocado posteriormente.
 */
VEC_TYPE VEC_FUN(_popAt)(VEC_NAME* const v, const size_t position) {
    VEC_TYPE toReturn = v->data[position];
    VEC_FUN(_moveBelow)(v, position);
    return toReturn;
}

/**
 * @brief           Apaga e dealoca o vetor.
 * @details         Dealoca todos os elementos do vetor utilizando o macto
 *                  'VEC_DEALOC', e dealoca o vetor em si, apagando-o.
 * @param v         Pointeiro para o vetor sob o qual operar.
 * @note            'v' em si não é dealocado.
 */
void VEC_FUN(_free)(VEC_NAME* const v) {
    for (size_t i = 0; i < v->size; i++) {
        VEC_DEALOC(v->data[i]);
    }
    if(v->alocated != 0) free(v->data);
    v->data = NULL;
    v->size = 0;
    v->alocated = 0;
}

/**
 * @brief           Remove um objeto do vetor.
 * @details         Remove, e dealoca o objeto na posição 'position' do vetor,
 *                  diminuindo o seu tamanho por uma unidade.
 * @param v         Pointeiro para o vetor sob o qual operar.
 * @param position  Index do elemento a remover.
 */
void VEC_FUN(_removeAt)(VEC_NAME* const v, size_t position) {
    VEC_DEALOC(v->data[position]);
    VEC_FUN(_moveBelow)(v, position);
}

/**
 * @brief           Ajusta o vetor para que tenha apenas o tamanho necessario
 *                  para guardar o seu conteudo.
 * @details         Ajusta a alocação do vetor para que esta seja igual ao seu
 *                  tamanho.
 * @param v         Pointeiro para o vetor sob o qual operar.
 * @returns         0 se falhou a redimensionar.
 * @returns         1 se redimensionou.
 * @returns         2 se não tiver que redimensionar.
 */
int VEC_FUN(_adjust)(VEC_NAME* const v) {
    if(v->size == v->alocated) return 2;
    VEC_TYPE* newData= realloc(v->data, v->size*sizeof(VEC_TYPE));
    if(newData == NULL) return 0;
    v->data = newData;
    v->alocated = v->size;
    return 1;
}

/**
 * @brief           Aplica a função 'predicate' a todos os elementos do vetor
 *                  'v', do menor ao maior index.
 * @details         O vetor 'v' é iterado, do primeiro ao último elemeto,
 *                  aplicando 'predicate' a todos os elementos até que o vetor
 *                  acabe ou até que 'predicate' retorne verdadeiro.
 * @param v         Pointeiro para o vetor sob o qual operar.
 * @param predicate Função que é chamada por cada elemento do vetor, se esta
 *                  função retornar verdadeiro a iteração pára. Os argumentos
 *                  desta função são: (1) o elemento que está a ser iterado;
 *                  (2) um ponteiro do tipo 'void*' que é passado ao chamar
 *                  _iterateFW.
 * @param userData  Dados passados pelo utilizador à função 'predicate'.
 * @returns         O index do objeto cuja função predicate primeiro returnou
 *                  verdade.
 * @returns         ~0 caso a todos os elementos foram iterados sem que
 *                  'predicate' tenha retornado 0.
 */
size_t VEC_FUN(_iterateFW)
(VEC_NAME* v, int(*predicate)(VEC_TYPE, void*), void* userData) {
    for (size_t i = 0; i < v->size; i++) {
        if( predidate(v->data[i], userData) ) return i;
    }
    return ~((size_t)0);
}

/**
 * @brief           Aplica a função 'predicate' a todos os elementos do vetor
 *                  'v', do maior ao menor index.
 * @details         O vetor 'v' é iterado, do último ao primeiro elemeto,
 *                  aplicando 'predicate' a todos os elementos até que o vetor
 *                  acabe ou até que 'predicate' retorne verdadeiro.
 * @param v         Pointeiro para o vetor sob o qual operar.
 * @param predicate Função que é chamada por cada elemento do vetor, se esta
 *                  função retornar verdadeiro a iteração pára. Os argumentos
 *                  desta função são: (1) o elemento que está a ser iterado;
 *                  (2) um ponteiro do tipo 'void*' que é passado ao chamar
 *                  _iterateBW.
 * @param userData  Dados passados pelo utilizador à função 'predicate'.
 * @returns         O index do objeto cuja função predicate primeiro returnou
 *                  verdade.
 * @returns         ~0 caso a todos os elementos foram iterados sem que
 *                  'predicate' tenha retornado 0.
 */
size_t VEC_FUN(_iterateBW)
(VEC_NAME* v, int(*predicate)(VEC_TYPE, void*), void* userData) {
    for (size_t i = v->size-1; i != ~((size_t)0); i--) {
        if( predidate(v->data[i], userData) ) return i;
    }
    return ~((size_t)0);
}

/**
 * @brief           Reserva espaço no vetor 'v'.
 * @details         Garante que o vetor 'v' tem no mínimo 'space' celulas de
 *                  memoria alocadas. Esta função é util quando queremos
 *                  adicionar um certo numero de objetos ao vetor sem ter que o
 *                  realocar múltiplas vezes.
 * @param v         Ponteiro para o vetor sob o qual queremos operar.
 * @param space     Numero de células de memória que queremos alocar.
 * @returns         0 se tentou mas não conseguiu alocar memória.
 * @returns         1 se alocou memoria.
 * @returns         2 se não o necessiou de alocar memória.
 * @warning         No final de chamar esta função o valor de células de memória
 *                  alocadas pode ser maior do que 'space'.
 */
int VEC_FUN(_reserve)(VEC_NAME* const v, size_t space) {
    if(v->alocated >= space) return 2;
    void* newData = realloc(v->data, sizeof(VEC_TYPE)*(space));
    if(newData == NULL) return 0;
    // Memoria alocada, fazer o update do vetor
    v->alocated = space;
    v->data = newData;
    return 1;
}

/**
 * @brief           Wrapper para que o macro 'VEC_DEALOC' possa ser chamado após
 *                  a inclusão do ficheio vector.h.
 * @param X         O parametro X do macro 'VEC_DEALOC'.
 */
void VEC_FUN(_DEALOC)(VEC_TYPE const X) {
    VEC_DEALOC(X);
}