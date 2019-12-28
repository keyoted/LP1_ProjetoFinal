#include "menu.h"

void cleanInputBuffer () {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

char* subStringTrimWhiteSpace(char *str) {
    if(!str) return NULL;

    // At the beguining
    while(isspace(*str)) str++;
    if(*str == 0)  // All spaces?
        return str;

    // At the end
    char* end = &str[strlen(str) - 1];
    while(end > str && isspace(*end)) end--;
    end[1] = '\0';

    return str;
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

    char* trimed = strdup(subStringTrimWhiteSpace(str));
    free(str);
    return trimed;
}

int menu_read_Float32 (_Float32* const value) {
    float f;
    if (scanf("%f", &f) != 1) {
        menu_printError("Não foi inserido um número válido.");
        cleanInputBuffer();
        return 0;
    }
    cleanInputBuffer();
    *value = (_Float32) f;
    return 1;
}

int menu_readInt (int* const value) {
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
    if( menu_readInt(op) ) {
        if(*op >= min) {
            if(*op <= max) {
                return 1;
            }
            else menu_printError("[%d] é maior que [%d].", *op, max);
        }
        else menu_printError("[%d] é menor que [%d].", *op, min);
    }
    return 0;
}

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
        menu_printDiv();
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
    if(!header) header = "";
    int spacesize = (80 - strlen(header)) - 8;
    if(spacesize < 3) spacesize = 3;
    for (int i = 0; i < spacesize/2; i++) printf(" ");
    printf("*** ");
    printf("%.66s", header);
    printf(" ***\n");
};

void menu_printEncomendaBrief(encomenda* e) {
    if(e->tipoEstado & ENCOMENDA_TIPO_URGENTE) printf("URGENTE ");
    else                                       printf("REGULAR ");

    switch (e->tipoEstado & 0xF0){
        case ENCOMENDA_ESTADO_EXPEDIDA:    printf("EXPEDIDA  "); break;
        case ENCOMENDA_ESTADO_CANCELADA:   printf("CANCELADA "); break;
        case ENCOMENDA_ESTADO_EM_ENTREGA:  printf("EM ENTREGA"); break;
        case ENCOMENDA_ESTADO_ENTREGUE:    printf("ENTREGUE  "); break;
    }

    printf(" (%.9s) ", e->NIF_cliente);
    struct tm* lt = localtime(&(e->criacao));
    printf(" %d/%d/%d %d:%d  -  ", 1900 + lt->tm_year, 1 + lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min);
    printf("%luc", encomenda_CalcPreco(e));
}

void menu_printUtilizador (utilizador u) {
    printf("%s (%.9s) ", u.nome, u.NIF);
    switch (u.tipo) {
        case UTILIZADOR_CLIENTE:
            printf("CLIENTE");
        break;
        case UTILIZADOR_DESATIVADO:
            printf("DESATIVADO");
        break;
        case UTILIZADOR_DIRETOR:
            printf("DIRETOR");
        break;
    }
}

void menu_printArtigo (artigo* a) {
    // nome gramas cm2 tratamento especial
    if(a->tratamentoEspecial) {
        printf("%s  -   %lug   %lucm2   * %s", a->nome, a->peso_gramas, a->cmCubicos, a->tratamentoEspecial);
    }
    else printf("%s  -   %lug   %lucm2   * N/A", a->nome, a->peso_gramas, a->cmCubicos);
}

void menu_printEncomendaDetail (encomenda* e) {
    menu_printDiv();
    menu_printDiv();
    menu_printHeader("Recibo de Encomenda");
    printf("*** NIF do Cliente: %.9s\n", e->NIF_cliente);

    struct tm* lt = localtime(&(e->criacao));
    printf("Data de criação: %d/%d/%d %d:%d", 1900 + lt->tm_year, 1 + lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min);

    menu_printHeader("Origem");
    printf("*** Morada: %s\n", e->origem.morada);
    printf("*** Código Postal: %.4s-%.3s\n", e->origem.codigoPostal, &(e->origem.codigoPostal[4]));

    menu_printHeader("Destino");
    printf("*** Morada: %s\n", e->destino.morada);
    printf("*** Código Postal: %.4s-%.3s\n", e->destino.codigoPostal, &(e->destino.codigoPostal[4]));


    menu_printHeader("Artigos");
    uint64_t pt = 0;
    uint64_t vt = 0;
    printf("*       ID | NOME                          | PESO     | VOLUME   | PESO T.  | VOL. T.  | * TRATAMENTO ESPECIAL\n");
    for(size_t i = 0; i < e->artigos.size; ++i) {
        printf("* %*lu |", 8, i+1);
        printf(" %*.*s |", 29, 29, e->artigos.data[i].nome);
        printf(" %*lu |", 8, e->artigos.data[i].peso_gramas);
        printf(" %*lu |", 8, e->artigos.data[i].cmCubicos);
        pt += e->artigos.data[i].peso_gramas;
        vt += e->artigos.data[i].cmCubicos;
        printf(" %*lu |", 8, pt);
        printf(" %*lu |", 8, vt);
        if(!e->artigos.data[i].tratamentoEspecial) printf(" N/A");
        else printf(" %s", e->artigos.data[i].tratamentoEspecial);
        printf("\n");
    }

    menu_printHeader("Outras Informações");
    printf("*** ESTADO: ");
    switch (e->tipoEstado & 0xF0){
        case ENCOMENDA_ESTADO_EXPEDIDA:    printf("EXPEDIDA\n"); break;
        case ENCOMENDA_ESTADO_CANCELADA:   printf("CANCELADA\n"); break;
        case ENCOMENDA_ESTADO_EM_ENTREGA:  printf("EM ENTREGA\n"); break;
        case ENCOMENDA_ESTADO_ENTREGUE:    printf("ENTREGUE\n"); break;
        default:                           printf("DESCONHECIDO\n");
    }

    printf("*** TIPO:        | ");
    if(e->tipoEstado & ENCOMENDA_TIPO_URGENTE)  printf("URGENTE    | ");
    else                                        printf("REGULAR    | ");
    if(e->tipoEstado & ENCOMENDA_TIPO_FRAGIL)   printf("FRAGIL     | ");
    else                                        printf("RESISTENTE | ");
    if(e->tipoEstado & ENCOMENDA_TIPO_PESADO)   printf("PESADO     | ");
    else                                        printf("LEVE       | ");
    if(e->tipoEstado & ENCOMENDA_TIPO_VOLUMOSO) printf("VOLUMOSO   |\n");
    else                                        printf("PEQUENO    |\n");

    uint64_t tpt = 0;
    printf("*** TIPO PREÇOS: | ");
    if(e->tipoEstado & ENCOMENDA_TIPO_URGENTE)  {printf(" %*luc | ", 8, e->precos.URGENTE); tpt += e->precos.URGENTE;}
    else                                        {printf(" %*luc | ", 8, e->precos.REGULAR); tpt += e->precos.REGULAR;}
    if(e->tipoEstado & ENCOMENDA_TIPO_FRAGIL)   {printf(" %*luc | ", 8, e->precos.FRAGIL);  tpt += e->precos.FRAGIL;}
    else                                         printf(" %*luc | ", 8, (uint64_t)0);
    if(e->tipoEstado & ENCOMENDA_TIPO_PESADO)   {printf(" %*luc | ", 8, e->precos.PESADO);  tpt += e->precos.PESADO;}
    else                                         printf(" %*luc | ", 8, (uint64_t)0);
    if(e->tipoEstado & ENCOMENDA_TIPO_VOLUMOSO) {printf(" %*luc |\n",8, e->precos.VOLUMOSO); tpt += e->precos.VOLUMOSO;}
    else                                         printf(" %*luc |\n",8, (uint64_t)0);

    printf("*** Preçco base: %luc\n", tpt);
    printf("*** Distancia: %luKm\n", e->distancia_km);
    printf("*** Preço Por Km: %luc\n", e->precos.POR_KM);
    const _Float32 multcp = e->precos.MULT_CP[e->origem.codigoPostal[0]-'0'][e->destino.codigoPostal[0]-'0'];
    printf("*** Multiplicador de Código Postal: %f\n", (double)multcp);
    printf("*** Preço Final em Cêntimos: %luc\n", encomenda_CalcPreco(e));
    menu_printDiv();
}

void menu_printReciboMensal (uint8_t NIF_USER[9], int mes, int ano, encomendavec* e) {
    size_t tot_enc = 0;
    size_t tot_art = 0;
    size_t tot_preco = 0;

    menu_printDiv();
    printf("*** NIF do Cliente: %.9s\n\n", NIF_USER);

    for(size_t i = 0; i < e->size; ++i) {
        struct tm* enctm = localtime(&e->data[i].criacao);
        if(enctm->tm_year != ano || enctm->tm_mon != mes || memcmp(NIF_USER, e->data[i].NIF_cliente, 9) != 0)
            continue;
        encomenda* atual = &(e->data[i]);
        menu_printEncomendaBrief(atual);
        printf("\n");
        size_t art;
        for(art = 0; art < atual->artigos.size; ++art) {
            printf("\t");
            menu_printArtigo(&(atual->artigos.data[art]));
            printf("\n");
        }
        tot_art += art;
        ++tot_enc;
        tot_preco += encomenda_CalcPreco(atual);
    }
    printf("\n");
    ano += 1900;
    mes += 1;
    printf("*** Encomendas feitas (%d/%d):    %lu\n",  ano, mes, tot_enc);
    printf("*** Artigos encomendados (%d/%d): %lu\n",  ano, mes, tot_art);
    printf("*** Preço final do mês (%d/%d):   %luc\n", ano, mes, tot_preco);
}