#include <stdint.h>

typedef struct {
    char*    nome;
    char*    tratamentoEspecial;
    uint64_t gramas;
    uint64_t milimetrosCubicos
} artigo;

int     ePesado    (artigo* a);
int     eVolumoso  (artigo* a);
artigo* newArtigo  ();
void    freeArtigo (artigo* a);