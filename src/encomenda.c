#include "encomenda.h"
#include "menu.h"

encomenda newEncomenda() {
    return (encomenda) {
        .artigos        = artigovec_new(),
        .origem         = newMorada(),
        .destino        = newMorada(),
        .criacao        = time(NULL)
    };
}

void freeEncomenda(encomenda* e) {
    artigovec_free(&(e->artigos));
    freeMorada(&(e->origem));
    freeMorada(&(e->destino));
}

int encomenda_ePesado(uint64_t a) {
    return a > 20000;
}

int encomenda_eVolumoso(uint64_t a) {
    return a > (50 * 50 * 50);
}

uint64_t encomenda_CalcPreco (encomenda* e) {
    uint64_t precoFinal;
    // Soma dos tipos de transporte
    // ENCOMENDA_TIPO_URGENTE
    if(e->tipoEstado & ENCOMENDA_TIPO_URGENTE) precoFinal = e->precos.URGENTE;
    else precoFinal = e->precos.REGULAR;
    // ENCOMENDA_TIPO_FRAGIL
    if(e->tipoEstado & ENCOMENDA_TIPO_FRAGIL) precoFinal += e->precos.FRAGIL;
    // ENCOMENDA_TIPO_PESADO
    if(e->tipoEstado & ENCOMENDA_TIPO_PESADO) precoFinal += e->precos.PESADO;
    // ENCOMENDA_TIPO_VOLUMOSO
    if(e->tipoEstado & ENCOMENDA_TIPO_VOLUMOSO) precoFinal += e->precos.VOLUMOSO;

    // Final do cálculo
    const _Float32 multcp = e->precos.MULT_CP[e->origem.codigoPostal[0]-'0'][e->destino.codigoPostal[0]-'0'];
    return (precoFinal + e->distancia_km*e->precos.POR_KM) * multcp;
}

void encomenda_TIPO_URGENTE (encomenda* e) {
    if(e->tipoEstado & ENCOMENDA_TIPO_URGENTE) {
        e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_URGENTE);
        menu_printInfo("encomenda definida como REGULAR");
    } else {
        e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_URGENTE;
        menu_printInfo("encomenda definida como URGENTE");
    }
}

void encomenda_TIPO_FRAGIL (encomenda* e) {
    for (size_t i = 0; i < e->artigos.size; i++) {
       if(e->artigos.data[i].tratamentoEspecial) {
           e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_FRAGIL;
           menu_printInfo("encomenda definida como FRAGIL");
           return;
       }
    }
    e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_FRAGIL);
    menu_printInfo("encomenda definida como RESISTENTE");
}

void encomenda_TIPO_FRAGIL_togle (encomenda* e) {
    if(!(e->tipoEstado & ENCOMENDA_TIPO_FRAGIL)) {
        // Encomenda não é fragil, logo pode ser posta como fragil manualmente
        e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_FRAGIL;
        menu_printInfo("encomenda definida como FRAGIL");
        return;
    }

    for (size_t i = 0; i < e->artigos.size; i++) {
       if(e->artigos.data[i].tratamentoEspecial) {
           // Encomenda é obigatoriamente fragil
           menu_printInfo("encomenda obrigatoriamente definida como FRAGIL");
           return;
       }
    }
    // Encomenda pode ser defenida como resistente
    e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_FRAGIL);
    menu_printInfo("encomenda definida como RESISTENTE");
}

void encomenda_TIPO_PESADO (encomenda* e) {
    uint64_t pesoAcumulado = 0;
    for (size_t i = 0; i < e->artigos.size; i++) {
        pesoAcumulado += e->artigos.data[i].peso_gramas;
    }
    if(encomenda_ePesado(pesoAcumulado)) {
        e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_PESADO;
        menu_printInfo("encomenda definida como PESADO");
    } else {
        e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_PESADO);
        menu_printInfo("encomenda definida como LEVE");
    }
}

void encomenda_TIPO_VOLUMOSO (encomenda* e) {
    uint64_t volumeAcumulado = 0;
    for (size_t i = 0; i < e->artigos.size; i++) {
        volumeAcumulado += e->artigos.data[i].cmCubicos;
    }
    if(encomenda_eVolumoso(volumeAcumulado)) {
        e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_VOLUMOSO;
        menu_printInfo("encomenda definida como VOLUMOSO");
    } else {
        e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_VOLUMOSO);
        menu_printInfo("encomenda definida como PEQUENO");
    }
}

int encomenda_generic_estado(encomenda* e, uint8_t paraMudar, uint8_t toggle) {
     if( (e->tipoEstado & 0xF0) == paraMudar) {
        // Se já estiver no estado para mudar, fazer toggle
        e->tipoEstado = (e->tipoEstado & 0x0F) | toggle;
        return 1;
    } else {
        e->tipoEstado = (e->tipoEstado & 0x0F) | paraMudar;
        return 0;
    }
}

void encomenda_ESTADO_EM_ENTREGA (encomenda* e) {
    if(encomenda_generic_estado(e, ENCOMENDA_ESTADO_EM_ENTREGA, ENCOMENDA_ESTADO_CANCELADA)) {
        menu_printInfo("encomenda definida como CANCELADA");
    } else {
        menu_printInfo("encomenda definida como EM ENTREGA");
    }
}

void encomenda_ESTADO_EXPEDIDA (encomenda* e) {
    if(encomenda_generic_estado(e, ENCOMENDA_ESTADO_EXPEDIDA, ENCOMENDA_ESTADO_EM_ENTREGA)) {
        menu_printInfo("encomenda definida como EM ENTREGA");
    } else {
        menu_printInfo("encomenda definida como EXPEDIDA");
    }
}

void encomenda_ESTADO_ENTREGUE (encomenda* e) {
    if(encomenda_generic_estado(e, ENCOMENDA_ESTADO_ENTREGUE, ENCOMENDA_ESTADO_EM_ENTREGA)) {
        menu_printInfo("encomenda definida como EM ENTREGA");
    } else {
        menu_printInfo("encomenda definida como ENTREGUE");
    }
}

void encomenda_ESTADO_CANCELADA (encomenda* e) {
    if(encomenda_generic_estado(e, ENCOMENDA_ESTADO_CANCELADA, ENCOMENDA_ESTADO_EM_ENTREGA)) {
        menu_printInfo("encomenda definida como EM ENTREGA");
    } else {
        menu_printInfo("encomenda definida como CANCELADA");
    }
}

encomenda encomenda_formalizar (artigovec artigos, precos_tt_cent precos, uint64_t ID_cliente, morada org, morada dest, uint64_t dist) {
    encomenda e;
    e.artigos = artigos;
    e.destino = dest;
    e.distancia_km = dist;
    e.ID_cliente = ID_cliente;
    e.origem = org;
    e.precos = precos;
    for (int i = 0; i < 10; i++) {
        memcpy(&(e.precos.MULT_CP[i]), &(precos.MULT_CP[i]), 10);
    }
    e.tipoEstado = ENCOMENDA_ESTADO_EM_ENTREGA;
    encomenda_TIPO_VOLUMOSO(&e);
    encomenda_TIPO_FRAGIL(&e);
    encomenda_TIPO_PESADO(&e);
    e.criacao = time(NULL);
    return e;
}

/*
    - encomenda encomendas
        - artigovec      artigos
            * uint64_t  tamanho_de_artigovec
            - artigo artigos[amanho_de_artigovec]
        - morada         origem
        - morada         destino
        * uint64_t       distancia_km
        * uint8_t        tipoEstado
        - precos_tt_cent precos
        * uint8_t        NIF_cliente[9]
        * time_t         criacao
*/
int save_encomenda (FILE* f, encomenda* data) {
    size_t written = 0;
    written += fwrite(&(data->artigos.size), sizeof(uint64_t), 1, f);
    if (data->artigos.size == 0) {
        menu_printError("ao gravar encomenda - encomenda sem artigos.");
        written = 0;
    }
    for(uint64_t i = 0; i < data->artigos.size; ++i) {
        written += save_artigo(f, &(data->artigos.data[i]));
    }
    written += save_morada(f, &(data->origem));
    written += save_morada(f, &(data->destino));
    written += fwrite(&(data->distancia_km), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->tipoEstado), sizeof(uint8_t), 1, f);
    written += save_precos(f, &(data->precos));
    written += fwrite(&(data->ID_cliente), sizeof(uint64_t), 1, f);
    written += fwrite(&(data->criacao), sizeof(time_t), 1, f);
    return written == (1 + data->artigos.size + 5 + 1 + 1);
}

int load_encomenda (FILE* f, encomenda* data) {
    size_t written = 0;
    uint64_t size_tmp = 0;
    written += fread(&(size_tmp), sizeof(uint64_t), 1, f);
    if (size_tmp == 0) {
        menu_printError("ao carregar encomenda - encomenda sem artigos.");
        written = 0;
    }
    data->artigos = artigovec_new();
    artigovec_reserve(&(data->artigos), size_tmp);
    for(uint64_t i = 0; i < size_tmp; ++i) {
        written += load_artigo(f, &(data->artigos.data[i]));
        ++data->artigos.size;
    }
    written += load_morada(f, &(data->origem));
    written += load_morada(f, &(data->destino));
    written += fread(&(data->distancia_km), sizeof(uint64_t), 1, f);
    written += fread(&(data->tipoEstado), sizeof(uint8_t), 1, f);
    written += load_precos(f, &(data->precos));
    written += fread(&(data->ID_cliente), sizeof(uint64_t), 1, f);
    written += fread(&(data->criacao), sizeof(time_t), 1, f);
    return written == (1 + size_tmp + 5 + 1 + 1);
}
