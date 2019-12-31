#ifndef PRECOS_TT_CENT_h
#define PRECOS_TT_CENT_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint64_t REGULAR ;
    uint64_t URGENTE ;
    uint64_t VOLUMOSO;
    uint64_t FRAGIL  ;
    uint64_t PESADO  ;
    uint64_t POR_KM  ;
    _Float32 MULT_CP [10][10]; // Origem Destino
} precos_tt_cent;

int save_precos (FILE* const f, const precos_tt_cent* const data);
int load_precos (FILE* const f, precos_tt_cent* const data);

#endif