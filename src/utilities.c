#include "utilities.h"

char* strdup (const char* const s) {
    if(!s) return NULL;
    size_t len = strlen(s)+1; // Make room for '\0'
    char* new;
    protectVarFcnCall(new, malloc(len), "strdup - alocação de memória recusada.");

    memcpy(new, s, len);
    return new;
}

int vecPrintItemPredicate (char* const item, int* const userdata) {
    printf("   %*d   |   %s\n", 8, ++(*userdata), protectStr(item));
    return 0;
}

/*
    para guardar str (char*)
        * uint32_t tamanho
        * char* data[]
        (SE tamanho == 0, retornar NULL ao ler)
*/
int save_str (FILE* const f, const char* const data) {
    if(!data) {
        const uint32_t ZERO = 0;
        fwrite(&ZERO, sizeof(uint32_t), 1, f);
        return 1;
    }
    uint32_t written = 0;
    uint32_t size = strlen(data);
    written += fwrite(&size, sizeof(uint32_t), 1, f);
    written += fwrite(data, sizeof(uint8_t), size, f);
    return written == (size + 1);
}

int load_str (FILE* const f, char** const data) {
    uint32_t written = 0;
    uint32_t size = 0;
    written += fread(&size, sizeof(uint32_t), 1, f);
    if(size == 0) {
        *data = NULL;
        return 1;
    }
    protectVarFcnCall(*data, malloc(size), "load_str - alocação de memória recusada.");
    written += fread(*data, sizeof(uint8_t), size, f);
    return written == (size + 1);
}