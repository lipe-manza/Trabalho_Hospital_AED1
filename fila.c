#include "fila.h"
#include <stdio.h>
#include <stdlib.h>

struct fila {
    PACIENTE *pacientes[TAM_FILA];
    int inicio;
    int fim;
    int tamanho;
};

FILA *fila_criar() {
    FILA *f = (FILA *)malloc(sizeof(FILA));
    if (f != NULL) {
        f->inicio = 0;
        f->fim = 0;
        f->tamanho = 0;
    }
    return f;
}

void fila_apagar(FILA **f) {
    if (f == NULL || *f == NULL) {
        return;
    }
    free(*f);
}

bool fila_vazia(FILA *f) {
    if (f == NULL) return true;

    if (f->tamanho == 0) return true;
    else return false;
}

bool fila_cheia(FILA *f) {
    if (f == NULL) return true;
    if (f->tamanho == TAM_FILA) return true;
    else return false;
}

int fila_tamanho(FILA *f) {
    if (f == NULL) return -1;
    return f->tamanho;
}

PACIENTE *fila_inicio(FILA *f) {
    if (f == NULL) return NULL;
    return f->pacientes[f->inicio];
}

bool fila_inserir(FILA *f, PACIENTE *paciente) {
    if (f == NULL || paciente == NULL) return false;
    if (fila_cheia(f)) return false;
    f->pacientes[f->fim] = paciente;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->tamanho++;
    return true;
}

PACIENTE *fila_remover(FILA *f) {
    if (f == NULL) return NULL;
    if (fila_vazia(f)) return NULL;
    PACIENTE *paciente = f->pacientes[f->inicio];
    f->pacientes[f->inicio] = NULL;
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->tamanho--;
    return paciente;
}

void fila_imprimir(FILA *f) {
    if (f == NULL || fila_vazia(f)) {
        printf("Fila vazia.\n");
        return;
    }
    int count = 0;
    int i = f->inicio;
    printf("=== Fila de Espera ===\n");
    while (count < f->tamanho) {
        paciente_imprimir(f->pacientes[i]);
        i = (i + 1) % TAM_FILA;
        count++;
    }
    printf("Total na fila: %d\n", f->tamanho);
}

bool fila_contem_paciente(FILA *f, int id) {
    if (f == NULL || fila_vazia(f)) return false;
    
    int count = 0;
    int i = f->inicio;
    while (count < f->tamanho) {
        if (paciente_get_id(f->pacientes[i]) == id) {
            return true;
        }
        i = (i + 1) % TAM_FILA;
        count++;
    }
    return false;
}

void load(FILA *f, char *filename) {
    //
}