#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lista.h"

typedef struct no {
    PACIENTE* paciente;
    NO* proximo;
    NO* anterior;
}NO;

struct lista {
    NO* inicio;
    NO* fim;
    int tamanho;
    bool ordenada;
};

LISTA* lista_criar() {
    LISTA* list = (LISTA*)malloc(sizeof(LISTA));
    if (list) {
        list->inicio = NULL;
        list->fim = NULL;
        list->tamanho = 0;
        list->ordenada = true;
    }
    return list;
}


bool lista_inserir_paciente(LISTA* list, PACIENTE* paciente) {
    if (list == NULL || paciente == NULL || lista_cheia(list)) {
        return false;
    }
    
    NO* novo = (NO*)malloc(sizeof(NO));
    if (novo == NULL) {
        return false;
    }
    
    novo->paciente = paciente;
    novo->proximo = NULL;
    novo->anterior = NULL;
    int id = paciente_get_id(paciente);
    
    // Lista vazia
    if (list->tamanho == 0) {
        list->inicio = novo;
        list->fim = novo;
        list->tamanho++;
        return true;
    }
    
    // Inserir no início (ID menor que o primeiro)
    if (id < paciente_get_id(list->inicio->paciente)) {
        novo->proximo = list->inicio;
        list->inicio->anterior = novo;
        list->inicio = novo;
        list->tamanho++;
        return true;
    }
    
    // Inserir no fim (ID maior que o último)
    if (id > paciente_get_id(list->fim->paciente)) {
        novo->anterior = list->fim;
        list->fim->proximo = novo;
        list->fim = novo;
        list->tamanho++;
        return true;
    }
    
    // Inserir no meio - busca a posição correta
    NO* atual = list->inicio;
    while (atual != NULL && paciente_get_id(atual->paciente) < id) {
        atual = atual->proximo;
    }
    
    // Não permitir IDs duplicados
    if (atual != NULL && paciente_get_id(atual->paciente) == id) {
        free(novo);
        return false;
    }
    
    // Inserir antes de 'atual'
    if (atual != NULL) {
        novo->proximo = atual;
        novo->anterior = atual->anterior;
        if (atual->anterior != NULL) {
            atual->anterior->proximo = novo;
        }
        atual->anterior = novo;
        list->tamanho++;
        return true;
    }

    free(novo);
    return false;
}


PACIENTE* lista_remover_paciente(LISTA* lista, int id) {
    if (lista == NULL || lista_vazia(lista)) {
        return NULL;
    }

    NO* atual = lista->inicio;
    
    while (atual != NULL && paciente_get_id(atual->paciente) < id) {
        atual = atual->proximo;
    }
    
    // Não encontrou ou passou do ID
    if (atual == NULL || paciente_get_id(atual->paciente) != id) {
        return NULL;
    }
    
    PACIENTE* paciente = atual->paciente;
    
    // Remover o nó usando ponteiros duplos
    if (atual->anterior != NULL) {
        atual->anterior->proximo = atual->proximo;
    } else {
        // É o primeiro nó
        lista->inicio = atual->proximo;
    }
    
    if (atual->proximo != NULL) {
        atual->proximo->anterior = atual->anterior;
    } else {
        // É o último nó
        lista->fim = atual->anterior;
    }
    
    free(atual);
    lista->tamanho--;
    
    return paciente;
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

PACIENTE* lista_buscar_paciente(LISTA* lista, int id) {
    if (lista == NULL || lista_vazia(lista)) {
        return NULL;
    }
    
    NO* atual = lista->inicio;
    
    while (atual != NULL) {
        int id_atual = paciente_get_id(atual->paciente);
        
        if (id_atual == id) {
            return atual->paciente;
        }
        
        // Lista ordenada: se passou do ID, não precisa continuar
        if (id_atual > id) {
            return NULL;
        }
        
        atual = atual->proximo;
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
    if (lista == NULL || lista_vazia(lista)) {
        printf("Lista vazia!\n");
        return;
    }
    
    NO* p = lista->inicio;
    printf("=== Lista de Pacientes Cadastrados (ordenada por ID) ===\n");
    while (p != NULL) {
        paciente_imprimir(p->paciente);
        printf("\n");
        p = p->proximo;
    }
    printf("Total de pacientes Cadastrados: %d\n", lista->tamanho);
}
