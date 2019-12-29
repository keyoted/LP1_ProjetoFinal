#ifndef OUTRASLISTAGENS_H
#define OUTRASLISTAGENS_H

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <time.h>

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

extern artigovec      artigos;         // Artigos da seção atual, que ainda não foram formalizados numa encomenda
extern encomendavec   encomendas;      // Encomendas formalizadas
utilizadorvec         utilizadores;    // Utilizadores existentes no registo, index 0 é diretor
extern size_t         utilizadorAtual; // Index do utilizador atual
extern precos_tt_cent tabelaPrecos;    // Preço em centimos por cada tipo de transporte

void listagem_Encomendas_Periodo_de_Tempo   ();
void listagem_Utilizadores_Mais_Gasto       ();
void listagem_Encomenda_EmEstado_PorPreco   ();
void listagem_Artigos_Semana                ();
void interface_outrasListagens              ();

#endif