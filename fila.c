#include "fila.h"
#include <stdlib.h>

struct fila {
    PACIENTE *pacientes[TAM];
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
    if (f->tamanho == TAM) return true;
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
    f->fim = (f->fim + 1) % TAM;
    f->tamanho++;
    return true;
}

PACIENTE *fila_remover(FILA *f) {
    if (f == NULL) return NULL;
    if (fila_vazia(f)) return NULL;
    PACIENTE *paciente = f->pacientes[f->inicio];
    f->pacientes[f->inicio] = NULL;
    f->inicio = (f->inicio + 1) % TAM;
    f->tamanho--;
    return paciente;
}

void fila_imprimir(FILA *f) {
    if (f == NULL) return;
    for (int i = f->inicio; i < f->fim; i++) {
        paciente_imprimir(f->pacientes[i]);
    }
}

void load(FILA *f, char *filename) {
    //
}