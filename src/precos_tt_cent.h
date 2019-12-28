#ifndef PRECOS_TT_CENT_h
#define PRECOS_TT_CENT_h

#include <inttypes.h>
#include <math.h>
#include <stdio.h>

typedef struct {
    uint64_t REGULAR ;
    uint64_t URGENTE ;
    uint64_t VOLUMOSO;
    uint64_t FRAGIL  ;
    uint64_t PESADO  ;
    uint64_t POR_KM  ;
    _Float32 MULT_CP [10][10]; // Origem Destino
} precos_tt_cent;

int save_precos (FILE* f, precos_tt_cent* data);
int load_precos (FILE* f, precos_tt_cent* data);

#endif