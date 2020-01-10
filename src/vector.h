/**
 * @file    vector.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Protótipo para um vetor, um objeto que contém um numero variável de
 *          objetos e que é capaz de alocar memoria dinâmicamente e de dealocar
 *          memória quando necessáro. Este ficheiro contém apenas um 'protótipo'
 *          que é compilado para um objeto real quando os macros VEC_TYPE;
 *          VEC_NAME e VEC_DEALOC(X) são definidos antes de incluir o ficheiro.
 *          VEC_IMPLEMENTATION e VEC_DECLARATION terão também que ser definidos
 *          (mesmo que sem nenhum valor) de modo a ativar a implementação e a
 *          declaração das funções que dizem respeito ao vetor que está a ser
 *          criado. Por omição da defenição de ambos os macros, VEC_DECLARATION
 *          é definido automaticamente.
 * @version 1
 * @date    2019-12-04
 *
 * @copyright Copyright (c) 2019
 * @section example_sec Exemplo
 * @code{c}
 * #ifndef  strvec_H
 * #define  strvec_H
 * #define  VEC_TYPE             char*
 * #define  VEC_NAME             strvec
 * #define  VEC_DEALOC(X)        free(X)
 * #include "./vector.h"
 * #endif
 *
 * #ifndef  intvec_H
 * #define  intvec_H
 * #define  VEC_TYPE             int
 * #define  VEC_NAME             intvec
 * #define VEC_WRITE(X, F)       fwrite(X, sizeof(int), 1, F)
 * #define VEC_READ(X, F)        fread (X, sizeof(int), 1, F)
 * #include "./vector.h"
 * #endif
 * @endcode
 */

/**
 * @def VEC_TYPE
 *                  O tipo de dados guardado no vetor.
 * @def VEC_NAME
 *                  Nome da nova struct, todas as funções têm este nome como
 *                  prefixo.
 * @def VEC_DEALOC(X)
 *                  Maneira de dealocar VEC_TYPE, se necessário
 *                  (pode não ser definido).
 * @def PASTER(X, Y)
 *                  Macro auxiliar para juntar VEC_NAME e Y.
 * @def EVAL(X, Y)
 *                  Macro auxiliar para avaliar VEC_NAME de modo a poder
 *                  juntá-lo com o nome da função.
 * @def VEC_FUN(X)
 *                  Macro responsavél por adicionar VEC_NAME antes do nome de
 *                  cada função.
 * @def VEC_IMPLEMENTATION
 *                  Se este macro estiver definido as implementações das funções
 *                  do vetor são ativadas.VEC_DECLARATION é definida por omição
 *                  de VEC_IMPLEMENTATION e VEC_DECLARATION.
 * @def VEC_DECLARATION
 *                  Se este macro estiver definido as declarações das funções do
 *                  vetor são ativadas. VEC_DECLARATION é definida por omição de
 *                  VEC_IMPLEMENTATION e VEC_DECLARATION.
 * @def INVAL_INDEX
 *                  O valor máximo que o tamanho do vetor pode ter.
 * @def VEC_WRITE(X, F)
 *                  Função para escrever o tipo 'VEC_TYPE' num ficheiro. Onde X
 *                  é do tipo 'VEC_TYPE*' e F é do tipo FILE* e corresponde ao
 *                  ficheiro onde gravar o objeto. Caso não esteja definido, a
 *                  função _write não vai ser gerada.
 * @def VEC_READ(X, F)
 *                  Função para carregar o tipo 'VEC_TYPE' num ficheiro. Onde X
 *                  é do tipo 'VEC_TYPE*' e F é do tipo FILE* e corresponde ao
 *                  ficheiro onde gravar o objeto. Caso não esteja definido, a
 *                  função _read não vai ser gerada.
 */

#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef VEC_TYPE
#    define VEC_TYPE int
#endif

#ifndef VEC_NAME
#    define VEC_NAME vector
#endif

#define PASTER(X, Y) X##Y
#define EVAL(X, Y) PASTER(X, Y)
#define VEC_FUN(X) EVAL(VEC_NAME, X)

#if defined(VEC_IMPLEMENTATION) || defined(VEC_DECLARATION)
#else
#    define VEC_DECLARATION
#endif

#define INVAL_INDEX ~((uint64_t) 0)

/**
 * @struct          VEC_NAME
 * @brief           Struct com o nome VEC_NAME que contém o tipo de dados
 *                  VEC_TYPE e informação sobre o numero de objetos guardados,
 *                  pode guardar [0, INVAL_INDEX[ elemntos.
 */
typedef struct {
    uint64_t  alocated; ///< Tamanho alocado de objetos.
    uint64_t  size;     ///< Tamanho de objetos que foi populado.
    VEC_TYPE* data;     ///< Começa em data[0] e acaba em data[size-1].
} VEC_NAME;
typedef int (*VEC_FUN(_pred_t))(VEC_TYPE*, void*);

#ifdef VEC_IMPLEMENTATION
/**
 * @brief           Aumenta v, se necessário e possivél, para que se possa
 *                  adicionar mais objetos no final.
 * @param v         Vetor a que queremos adicionar espaço.
 * @returns         0 se tentou mas não conseguiu alocar memória.
 * @returns         1 se alocou memória.
 * @returns         2 se não necessitou de alocar memoria.
 */
int VEC_FUN(_addCell)(VEC_NAME* const v) {
    if (v->alocated == 0) {
        // Vetor vazio
        v->data = malloc(sizeof(VEC_TYPE) * 8);
        if (v->data == NULL) return 0;
        v->alocated = 8;
        return 1;
    } else if (v->size == v->alocated) {
        // O vetor está cheio e necessita de ser redimensionado
        void* newData = realloc(v->data, sizeof(VEC_TYPE) * (v->alocated * 2));
        if (newData == NULL) {
            // Foi impossivel realocar memoria sufuciente
            // Tentar realocar apenas mais um espaço
            newData = realloc(v->data, sizeof(VEC_TYPE) * (v->alocated + 1));
            if (newData == NULL) {
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
    } else {
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
VEC_NAME VEC_FUN(_new)() { return (VEC_NAME) {.size = 0, .alocated = 0, .data = NULL}; }

/**
 * @brief           Adiciona um objeto no final do vetor.
 * @param v         Ponteiro para o vetor sob o qual operar.
 * @param newObj    Objeto para adicionar ao vetor.
 * @returns         0 se não conseguiu inserir o objeto.
 * @returns         1 se conseguiu inserir o objeto.
 */
int VEC_FUN(_push)(VEC_NAME* const v, VEC_TYPE const newObj) {
    if (!VEC_FUN(_addCell)(v)) return 0;
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
void VEC_FUN(_moveBelow)(VEC_NAME* const v, const uint64_t i) {
    memmove(&v->data[i], &v->data[i + 1], (v->size - i - 1) * sizeof(VEC_TYPE));
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
int VEC_FUN(_moveAbove)(VEC_NAME* const v, const uint64_t i) {
    if (!VEC_FUN(_addCell)(v)) return 0;
    memmove(&v->data[i + 1], &v->data[i], (v->size - i) * sizeof(VEC_TYPE));
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
int VEC_FUN(_pushAt)(VEC_NAME* const v, VEC_TYPE const newObj, const uint64_t position) {
    if (!VEC_FUN(_moveAbove)(v, position)) return 0;
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
    VEC_TYPE toReturn = v->data[v->size - 1];
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
VEC_TYPE VEC_FUN(_popAt)(VEC_NAME* const v, const uint64_t position) {
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
#    ifdef VEC_DEALOC
    for (uint64_t i = 0; i < v->size; i++) { VEC_DEALOC(v->data[i]); }
#    endif
    if (v->alocated != 0) free(v->data);
    v->data     = NULL;
    v->size     = 0;
    v->alocated = 0;
}

/**
 * @brief           Remove um objeto do vetor.
 * @details         Remove, e dealoca o objeto na posição 'position' do vetor,
 *                  diminuindo o seu tamanho por uma unidade.
 * @param v         Pointeiro para o vetor sob o qual operar.
 * @param position  Index do elemento a remover.
 */
void VEC_FUN(_removeAt)(VEC_NAME* const v, uint64_t position) {
#    ifdef VEC_DEALOC
    VEC_DEALOC(v->data[position]);
#    endif
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
    if (v->size == v->alocated) return 2;
    VEC_TYPE* newData = realloc(v->data, v->size * sizeof(VEC_TYPE));
    if (newData == NULL) return 0;
    v->data     = newData;
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
 * @returns         INVAL_INDEX caso a todos os elementos foram iterados sem que
 *                  'predicate' tenha retornado 0.
 */
uint64_t VEC_FUN(_iterateFW)(VEC_NAME* const v, VEC_FUN(_pred_t) predicate, void* userData) {
    for (uint64_t i = 0; i < v->size; i++) {
        if (predicate(&(v->data[i]), userData)) return i;
    }
    return INVAL_INDEX;
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
 * @returns         INVAL_INDEX caso a todos os elementos foram iterados sem que
 *                  'predicate' tenha retornado 0.
 */
uint64_t VEC_FUN(_iterateBW)(VEC_NAME* const v, VEC_FUN(_pred_t) predicate, void* userData) {
    for (uint64_t i = v->size - 1; i != INVAL_INDEX; i--) {
        if (predicate(&(v->data[i]), userData)) return i;
    }
    return INVAL_INDEX;
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
int VEC_FUN(_reserve)(VEC_NAME* const v, uint64_t space) {
    if (v->alocated >= space) return 2;
    void* newData = realloc(v->data, sizeof(VEC_TYPE) * (space));
    if (newData == NULL) return 0;
    // Memoria alocada, fazer o update do vetor
    v->alocated = space;
    v->data     = newData;
    return 1;
}

/**
 * @brief           Wrapper para que o macro 'VEC_DEALOC' possa ser chamado após
 *                  a inclusão do ficheio vector.h.
 * @param X         O parametro X do macro 'VEC_DEALOC'.
 */
#    ifdef VEC_DEALOC
void VEC_FUN(_DEALOC)(VEC_TYPE* const X) { VEC_DEALOC(*X); }
#    endif

#    ifdef VEC_WRITE
/**
 * @brief           Escreve num ficheiro utilizando o macro 'VEC_WRITE'.
 * @details         Escreve um número de 64 bits a indicar o tamanho do vetor e
 *                  de seguida escreve, um a um, utilizando o macro 'VEC_WRITE'
 *                  os objetos do vetor.
 * @param v         Ponteiro para o vetor sob o qual queremos operar.
 * @param f         Ficheiro onde escrever os conteudos do vetor.
 * @returns         1 se escreveu o vetor com sucesso.
 * @returns         0 caso contrário.
 */
int VEC_FUN(_write)(const VEC_NAME* const v, FILE* f) {
    // Gravar tamanho do vetor
    if (!fwrite(&v->size, sizeof(uint64_t), 1, f)) return 0;
    // Guardar objetos
    for (uint64_t i = 0; i < v->size; i++) {
        if (!VEC_WRITE(&(v->data[i]), f)) return 0;
    }
    return 1;
}
#    endif

#    ifdef VEC_READ
/**
 * @brief           Lê de num ficheiro utilizando o macro 'VEC_READ'.
 * @details         Lê um número de 64 bits a indicar o tamanho do vetor e
 *                  de seguida lê, um a um, utilizando o macro 'VEC_READ' os
 *                  objetos do vetor. Aloca espaço, se necessário.
 * @param v         Ponteiro para o vetor sob o qual queremos operar.
 * @param f         Ficheiro onde escrever os conteudos do vetor.
 * @returns         1 se leu o vetor com sucesso.
 * @returns         0 caso contrário.
 */
int VEC_FUN(_read)(VEC_NAME* const v, FILE* f) {
    // Ler tamanho de vetor em ficheiro
    uint64_t size = 0;
    if (!fread(&size, sizeof(uint64_t), 1, f)) return 0;
    // Reservar espaço para vetor
    if (!VEC_FUN(_reserve)(v, size)) return 0;
    // Ler objetos do ficheiro
    for (uint64_t i = 0; i < size; i++) {
        if (!VEC_READ(&(v->data[i]), f)) return 0;
        v->size++;
    }
    return 1;
}
#    endif
#endif

#ifdef VEC_DECLARATION
VEC_NAME VEC_FUN(_new)();
VEC_TYPE VEC_FUN(_pop)(VEC_NAME* const v);
void     VEC_FUN(_free)(VEC_NAME* const v);
int      VEC_FUN(_adjust)(VEC_NAME* const v);
int      VEC_FUN(_addCell)(VEC_NAME* const v);
int      VEC_FUN(_reserve)(VEC_NAME* const v, uint64_t space);
int      VEC_FUN(_push)(VEC_NAME* const v, VEC_TYPE const newObj);
void     VEC_FUN(_moveBelow)(VEC_NAME* const v, const uint64_t i);
void     VEC_FUN(_removeAt)(VEC_NAME* const v, uint64_t position);
int      VEC_FUN(_moveAbove)(VEC_NAME* const v, const uint64_t i);
VEC_TYPE VEC_FUN(_popAt)(VEC_NAME* const v, const uint64_t position);
uint64_t VEC_FUN(_iterateFW)(VEC_NAME* const v, VEC_FUN(_pred_t) predicate, void* userData);
uint64_t VEC_FUN(_iterateBW)(VEC_NAME* const v, VEC_FUN(_pred_t) predicate, void* userData);
#    ifdef VEC_DEALOC
void VEC_FUN(_DEALOC)(VEC_TYPE* const X);
#    endif
#    ifdef VEC_WRITE
int VEC_FUN(_write)(const VEC_NAME* const v, FILE* f);
#    endif
#    ifdef VEC_READ
int VEC_FUN(_read)(VEC_NAME* const v, FILE* f);
#    endif
#endif

#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#undef VEC_READ
#undef VEC_WRITE