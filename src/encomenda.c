#include "encomenda.h"
#include "menu.h"

encomenda newEncomenda() {
    return (encomenda) {
        .artigos        = artigovec_new(),
        .origem         = newMorada(),
        .destino        = newMorada(),
    };
}

void freeEncomenda(encomenda e) {
    artigovec_free(&(e.artigos));
    freeMorada(e.origem);
    freeMorada(e.destino);
}

int encomenda_ePesado(uint64_t a) {
    return a > 20000;
}

int encomenda_eVolumoso(uint64_t a) {
    return a > (500 * 500 * 500);
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
    return (precoFinal + e->distancia_km*e->precos.POR_KM) * e->precos.MULT_CP;
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
        volumeAcumulado += e->artigos.data[i].milimetrosCubicos;
    }
    if(encomenda_eVolumoso(volumeAcumulado)) {
        e->tipoEstado = e->tipoEstado | ENCOMENDA_TIPO_VOLUMOSO;
        menu_printInfo("encomenda definida como VOLUMOSO");
    } else {
        e->tipoEstado = e->tipoEstado & (~ENCOMENDA_TIPO_VOLUMOSO);
        menu_printInfo("encomenda definida como PEQUENO");
    }
}

void encomenda_ESTADO_EM_ENTREGA (encomenda* e) {
    // Apagar flags de estado
    e->tipoEstado = e->tipoEstado & (ENCOMENDA_TIPO_URGENTE | ENCOMENDA_TIPO_FRAGIL | ENCOMENDA_TIPO_PESADO | ENCOMENDA_TIPO_VOLUMOSO);
    // Acionar flag apropriada
    e->tipoEstado = e->tipoEstado | ENCOMENDA_ESTADO_EM_ENTREGA;
    menu_printInfo("encomenda definida como EM ENTREGA");
}

void encomenda_ESTADO_EXPEDIDA (encomenda* e) {
    // Apagar flags de estado
    e->tipoEstado = e->tipoEstado & (ENCOMENDA_TIPO_URGENTE | ENCOMENDA_TIPO_FRAGIL | ENCOMENDA_TIPO_PESADO | ENCOMENDA_TIPO_VOLUMOSO);
    // Acionar flag apropriada
    e->tipoEstado = e->tipoEstado | ENCOMENDA_ESTADO_EXPEDIDA;
    menu_printInfo("encomenda definida como EXPEDIDA");
}

void encomenda_ESTADO_ENTREGUE (encomenda* e) {
    // Apagar flags de estado
    e->tipoEstado = e->tipoEstado & (ENCOMENDA_TIPO_URGENTE | ENCOMENDA_TIPO_FRAGIL | ENCOMENDA_TIPO_PESADO | ENCOMENDA_TIPO_VOLUMOSO);
    // Acionar flag apropriada
    e->tipoEstado = e->tipoEstado | ENCOMENDA_ESTADO_ENTREGUE;
    menu_printInfo("encomenda definida como ENTREGUE");
}

void encomenda_ESTADO_CANCELADA (encomenda* e) {
    // Apagar flags de estado
    e->tipoEstado = e->tipoEstado & (ENCOMENDA_TIPO_URGENTE | ENCOMENDA_TIPO_FRAGIL | ENCOMENDA_TIPO_PESADO | ENCOMENDA_TIPO_VOLUMOSO);
    // Acionar flag apropriada
    e->tipoEstado = e->tipoEstado | ENCOMENDA_ESTADO_CANCELADA;
    menu_printInfo("encomenda definida como CANCELADA");
}

encomenda encomenda_formalizar (artigovec artigos, precos_tt_cent precos, float mult_CP[10][10], utilizador dest, morada org, uint64_t dist) {
    encomenda e;
    e.artigos = artigos;
    e.destino = dest.adereco;
    e.distancia_km = dist;
    memcpy(e.NIF_cliente, dest.NIF, 9);
    e.origem = org;
    e.precos = precos;
    e.precos.MULT_CP = mult_CP[e.origem.codigoPostal[0]-'0'][e.destino.codigoPostal[0]-'0'];
    e.tipoEstado = ENCOMENDA_ESTADO_EM_ENTREGA;
    encomenda_TIPO_VOLUMOSO(&e);
    encomenda_TIPO_FRAGIL(&e);
    encomenda_TIPO_PESADO(&e);
    return e;
}