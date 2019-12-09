#include "menu.h"

void cleanInputBuffer(){
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

char* inputString (FILE* fp) {
    size_t alocado = 20;
    size_t tamanho = 0;
    char* str = malloc(sizeof(char)*alocado);

    int ch;
    while ( (ch=fgetc(fp)) != EOF && ch != '\n' ) {
        str[tamanho++] = ch;
        if(tamanho == alocado) {
            char* tmp = realloc(str, sizeof(char)*(alocado+=20));
            if(!tmp) break;
            str = tmp;
        }
    }
    str[tamanho++] = '\0';
    char* tmp = realloc(str, sizeof(char)*(tamanho));
    if(tmp) return tmp;
    else return str;
}

int readIntFromStdin(int* const value) {
    if (scanf("%d", value) != 1) {
        printf("*** ERRO: Não foi inserido um número válido.\n");
        cleanInputBuffer();
        return 0;
    }
    cleanInputBuffer();
    return 1;
}

int lerNumeroEntre(int min, int max, int* const op) {
    printf("insira um numero entre [%d e %d] $ ", min, max);
    if( readIntFromStdin(op) ) {
        if(*op >= min) {
            if(*op <= max) {
                return 1;
            }
            else printf("*** ERRO: [%d] não é menor ou igual a [%d].\n", *op, max);
        }
        else printf("*** ERRO: [%d] é menor que [%d].\n", *op, min);
    }
    return 0;
}

int menu_selecao_display_iter(char* item, void* userdata) {
    printf("   %*d   |   %s\n", 8, ++(*((int*)userdata)), item);
    return 0;
}

// itens, separados por \n
void menu_selecao_display(const strVec itens) {
    printf("--------------------------------------------------------------------------------\n");
    printf("                         *** Selecione a sua opção ***                          \n");
    printf("   Opção      |   Item\n");
    printf("         -2   |   Reimprimir\n");
    printf("         -1   |   Sair\n");
    int i = -1;
    strVec_iterateFW((strVec*)(&itens), &menu_selecao_display_iter, &i);
    printf("--------------------------------------------------------------------------------\n");
}

// itens, separados por \n
// retorna o numero selecionado
int menu_selecao(const strVec itens) {
    int op = -2;
    int max = itens.size;
    while(op == -2) {
        menu_selecao_display(itens);
        while (!lerNumeroEntre(-2, max-1, &op));
    }
    return op;
}