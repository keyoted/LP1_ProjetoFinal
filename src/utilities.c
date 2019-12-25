#include "utilities.h"

char* strdup (const char *s) {
    size_t len = strlen (s);
    void *new = malloc (len);
    if (new == NULL)
        return NULL;
    return (char *) memcpy (new, s, len);
}

int vecPrintItemPredicate (char* item, int* userdata) {
    printf("   %*d   |   %s\n", 8, ++(*userdata), item);
    return 0;
}

/*
    para guardar str (char*)
        * uint32_t tamanho
        * char* data[]
        (SE tamanho == 0, retornar NULL ao ler)
*/
int save_str (FILE* f, uint8_t* data) {
    if(!data) {
        const uint32_t zero = 0;
        fwrite(&zero, sizeof(uint32_t), 1, f);
        return 1;
    }
    int saved = 0;
    uint32_t size = strlen(data);
    saved += fwrite(&size, sizeof(uint32_t), 1, f);
    saved += fwrite(data, sizeof(uint8_t), size, f);
    return saved == (size + 1);
}
