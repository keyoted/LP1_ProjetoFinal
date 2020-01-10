/**
 * @file    outrasListagens.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   As 5 listagens extra propostas pelos alunos.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#ifndef OUTRASLISTAGENS_H
#define OUTRASLISTAGENS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "artigo.h"
#include "encomenda.h"
#include "precos_cent.h"
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

artigovec     artigos;         ///< Artigos da seção atual, que ainda não foram formalizados numa encomenda.
encomendavec  encomendas;      ///< Encomendas formalizadas.
utilizadorvec utilizadores;    ///< Utilizadores existentes no registo, index 0 é sempre o diretor.
precos_cent   tabelaPrecos;    ///< Preço em centimos do transporte.
size_t        utilizadorAtual; ///< Index do utilizador atual.
void          funcional_consultar_estados_encomendas();


void listagem_Encomendas_Periodo_de_Tempo();
void listagem_Utilizadores_Mais_Gasto();
void listagem_Encomenda_EmEstado_PorPreco();
void listagem_Artigos_Semana();
void listagem_imprimir_recibo();
void interface_outrasListagens();

#endif