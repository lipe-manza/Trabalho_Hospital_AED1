#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "paciente.h"


typedef struct paciente PACIENTE;

struct paciente {
    int id;
    char nome[100];
    PILHA* historico;
};

PACIENTE* paciente_criar(int id, const char* nome) {
    PACIENTE* p = (PACIENTE*)malloc(sizeof(PACIENTE));
    if (p != NULL) {
        p->id = id;
        strncpy(p->nome, nome, sizeof(p->nome) - 1);
        strcat(p->nome, "\0");
        p->historico = pilha_criar();
        if (p->historico != NULL) {
            return p;
        }
        return NULL;
    }
    return NULL;
}

bool paciente_apagar(PACIENTE** p) {
    if (p != NULL && *p != NULL) {
        pilha_apagar(&(*p)->historico);
        free(*p);
        *p = NULL;
        return true;
    }
    return false;
}

int paciente_get_id(PACIENTE* p) {
    if (p != NULL) {
        return p->id;
    }
    return -1; // Indica erro
}

const char* paciente_get_name(PACIENTE* p) {
    if (p != NULL) {
        return p->nome;
    }
    return NULL; // Indica erro
}

bool paciente_add_medicamento(PACIENTE* p, char* medicamento) {
    if (p != NULL && medicamento != NULL) {
        bool success;
        success = pilha_empilhar(p->historico, medicamento);
        if (success) {
            return true;
        }
        return false;
    }
    return false;
}
bool paciente_remover_medicamento(PACIENTE* p) {
    if (p != NULL) {
        char* medicamento = pilha_desempilhar(p->historico);
        if (medicamento != NULL) {
            return true;
        }
        return false;
    }
    return false;
}

char* paciente_retirar_ultimo_medicamento(PACIENTE* p) {
    if (p != NULL) {
        return pilha_desempilhar(p->historico);
    }
    return NULL;
}

bool paciente_imprimir_historico(PACIENTE* p) {
    if (p != NULL) {
        pilha_imprimir(p->historico);
        return true;
    }
    return false;
}

void paciente_imprimir(PACIENTE* p) {
    if (p != NULL) {
        printf(" %d - %s\n", p->id, p->nome);
    }
}