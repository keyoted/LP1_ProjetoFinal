#include "menu.h"

void cleanInputBuffer () {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

char* menu_readString (FILE* fp) {
    size_t alocated = 20;
    size_t size = 0;
    char* str = malloc(sizeof(char)*alocated);

    int ch;
    while ( (ch=fgetc(fp)) != EOF && ch != '\n' ) {
        str[size++] = ch;
        if(size == alocated) {
            char* tmp = realloc(str, sizeof(char)*(alocated+=20));
            if(!tmp) break;
            str = tmp;
        }
    }
    str[size++] = '\0';

    // Trim
    size_t start;
    size_t end = size - 1;
    for (start = 0;    start < size          && isspace(str[start]);  start++);
    for (end = size-2; (end != ~((size_t)0)) && isspace(str[end]);    end--);
    str[end+1] = '\0';
    end+=2;

    char* tmp = malloc(sizeof(char)*(end-start));
    if(tmp) {
        memcpy(tmp, &str[start], sizeof(char)*(end-start));
        return tmp;
    }
    else return str;
}

int readInt (int* const value) {
    if (scanf("%d", value) != 1) {
        menu_printError("Não foi inserido um número válido.");
        cleanInputBuffer();
        return 0;
    }
    cleanInputBuffer();
    return 1;
}

int menu_readIntMinMax (int min, int max, int* const op) {
    printf("Insira um numero entre [%d e %d] $ ", min, max);
    if( readInt(op) ) {
        if(*op >= min) {
            if(*op <= max) {
                return 1;
            }
            else menu_printError("[%d] não é menor ou igual a [%d].", *op, max);
        }
        else menu_printError("[%d] é menor que [%d].", *op, min);
    }
    return 0;
}

// itens, separados por \n
// retorna o numero selecionado
int menu_selection (const strvec* itens) {
    int op = -2;
    int max = itens->size;
    while(op == -2) {
        printf("   Opção      |   Item\n");
        printf("         -2   |   Reimprimir\n");
        printf("         -1   |   Sair\n");
        int i = -1;
        strvec_iterateFW((strvec*)itens, (strvec_predicate_t)&vecPrintItemPredicate, &i);
        while (!menu_readIntMinMax(-2, max-1, &op));
    }
    return op;
}

void  menu_printDiv () {
    printf("--------------------------------------------------------------------------------\n");
}

void  menu_printError (char* err, ...) {
    va_list args;
    printf("*** ERRO: ");
    va_start(args, err);
    vprintf(err, args);
    va_end(args);
    printf("\n");
}

void  menu_printInfo (char* info, ...) {
    va_list args;
    printf("*** INFO: ");
    va_start(args, info);
    vprintf(info, args);
    va_end(args);
    printf("\n");
}

void  menu_printHeader (char* header) {
    int spacesize = (80 - strlen(header)) - 8;
    if(spacesize < 3) spacesize = 3;
    for (int i = 0; i < spacesize/2; i++) printf(" ");
    printf("*** ");
    printf("%.66s", header);
    printf(" ***\n");
};