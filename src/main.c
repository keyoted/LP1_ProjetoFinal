#include <stdio.h>
#include <string.h>


#define VEC_TYPE char*
#define VEC_NAME strvec
#define VEC_DEALOC(X) printf("Supostamente iria apagar '%s'\n", X)
#define VEC_CMP(X, Y, Z) !strcmp(*X, *Y)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#undef VEC_CMP

#define VEC_TYPE int
#define VEC_NAME intvec
#define VEC_DEALOC(X)
#define VEC_CMP(X, Y, Z) (*X) == (*Y)
#include "./vector.h"
#undef VEC_TYPE
#undef VEC_NAME
#undef VEC_DEALOC
#undef VEC_CMP




int main() {
    strvec sv = strvec_new();
    strvec* s = &sv;

    strvec_push(s, " 0 ");
    strvec_push(s, " 1 ");
    strvec_push(s, " 2 ");
    strvec_push(s, " 3 ");
    strvec_push(s, " 4 ");
    strvec_push(s, " 5 ");
    strvec_push(s, " 6 ");
    strvec_push(s, " 7 ");
    strvec_push(s, " 8 ");
    strvec_push(s, " 9 ");
    strvec_push(s, " 10 ");
    strvec_push(s, " 11 ");
    strvec_push(s, " 12 ");

    strvec_removeAt(s, 12);

    for(size_t i=0; i < sv.size; ++i) {
        printf("%s", sv.data[i]);
    }
    printf("\n");
    strvec_removeAt(s, 11);

    char* find = " 2 ";
    printf("%zu\n", strvec_find(s, &find, NULL));
    strvec_free(s);
    return 0;
}