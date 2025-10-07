#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lista.h"

typedef struct no {
    PACIENTE* paciente;
    NO* proximo;
}NO;

struct lista {
    NO* inicio;
    NO* fim;
    int tamanho;
};

LISTA* lista_criar() {
    LISTA* list = (LISTA*)malloc(sizeof(LISTA));
    if (list) {
        list->inicio = NULL;
        list->fim = NULL;
        list->tamanho = 0;
    }
    return list;
}

bool lista_inserir_fim(LISTA* list, PACIENTE* paciente) {
    NO* p = (NO*)malloc(sizeof(NO));
    if (p) {
        p->paciente = paciente;
        p->proximo = NULL;
        if (list->tamanho == 0) {
            list->inicio = p;
        }
        else {
            list->fim->proximo = p;
        }
        list->tamanho++;
        list->fim = p;
        return true;
    }
    return false;
}
bool lista_inserir(LISTA* list, PACIENTE* paciente) {
    bool x = false;
    if (list != NULL && !(lista_cheia(list))) {
        x = lista_inserir_fim(list, paciente);
        return x;
    }
    return false;
}


PACIENTE* lista_remover_paciente(LISTA* lista, int id) {
    NO* a;
    NO* p;
    PACIENTE* paciente;

    if (lista && !lista_vazia(lista)) {
        p = lista->inicio;
        while (p != NULL && paciente_get_id(p->paciente) != id) {
            a = p;
            p = p->proximo;
        }
        if (p != NULL) {
            if (p == lista->inicio) {
                lista->inicio = p->proximo;
            }
            else {
                a->proximo = p->proximo;
            }
            if (p == lista->fim) {
                lista->fim = a;
            }
            paciente = p->paciente;
            p->proximo = NULL;
            free(p);
            lista->tamanho--;
            return paciente;
        }
    }
    return NULL;
}

bool lista_apagar(LISTA** lista) {
    NO* p;
    NO* a;

    if (lista!=NULL && *lista!=NULL) {
        p = (*lista)->inicio;
        while (p != NULL) {
            a = p;
            p = p->proximo;
            free(a);
            a=NULL;
        }
        free(*lista);
        *lista = NULL;
        return true;
    }
    return false;
}

PACIENTE* lista_busca_paciente_rec(NO* n, int id) {
    if (n == NULL) {
        return NULL;
    }
    if (paciente_get_id(n->paciente) == id) {
        return n->paciente;
    }
    else {
        return lista_busca_paciente_rec(n->proximo, id);
    }

}

PACIENTE* lista_busca(LISTA* lista, int id) {
    PACIENTE* paciente;
    if (lista != NULL) {
        paciente = lista_busca_paciente_rec(lista->inicio, id);
        return paciente;
    }
    return NULL;
}

int lista_tamanho(LISTA* lista) {
    if (lista != NULL) {
        return lista->tamanho;
    }
    return 0;
}

bool lista_vazia(LISTA* lista) {
    if (lista != NULL) {
        if (lista->tamanho == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

bool lista_cheia(LISTA* lista) {
    if (lista != NULL) {
        NO* p = (NO*)malloc(sizeof(NO));
        if (p != NULL) {
            free(p);
            return false;
        }
        else return true;
    }
    return false;
}

void lista_imprimir_pacientes(LISTA* lista) {
    NO* p;
    if (lista != NULL && !lista_vazia(lista)) {
        p = lista->inicio;
        while (p != NULL) {
            paciente_imprimir(p->paciente);
            printf("\n");
            p = p->proximo;
        }
    }
    printf("Lista vazia!");
}
