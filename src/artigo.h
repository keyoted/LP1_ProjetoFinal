#ifndef ARTIGO_H
#define ARTIGO_H

#include <stdint.h>
#include "utilities.h"

enum ARTIGOS_TIPOS {
     ARTIGO_REGULAR  = 0x1,
     ARTIGO_URGENTE  = 0x2,
     ARTIGO_FRAGIL   = 0x4,
     ARTIGO_PESADO   = 0x8,
     ARTIGO_VOLUMOSO = 0x10,
};

enum precoTipoDeTransporte {           // Enum para auxiliar com a indexação de tabelaPrecoTipoTransporte_cent
     TABELA_ARTIGO_REGULAR  = 0,
     TABELA_ARTIGO_URGENTE  = 1,
     TABELA_ARTIGO_FRAGIL   = 2,
     TABELA_ARTIGO_PESADO   = 3,
     TABELA_ARTIGO_VOLUMOSO = 4,
};

typedef struct {
    char*    nome;                          // Nome do artigo
    char*    tratamentoEspecial;            // Texto sobre o tratamento especial do artigo
    uint8_t  tipoDeTransporte;              // Tipo de transporte
    uint64_t precoTipoDeTransporte_cent;    // Preço consuante o tipo de transporte selecionado na altura
    uint64_t peso_gramas;                   // Peso do artigo em gramas
    uint64_t milimetrosCubicos;             // Volume do artigo em milimetros cubicos
} artigo;

int      artigo_ePesado             (artigo a);
int      artigo_eVolumoso           (artigo a);
artigo   newArtigo                  ();
void     freeArtigo                 (artigo a);
uint64_t artigo_precoTipoTransporte (artigo a, uint64_t tabelaPrecos[5]);

#endif