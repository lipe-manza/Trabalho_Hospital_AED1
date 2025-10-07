#include "pilha.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct pilha {
  char historico[TAM][101];
  int topo;
};

PILHA *pilha_criar() {
  PILHA *p = (PILHA *)malloc(sizeof(PILHA));
  if (p != NULL) {
    p->topo = -1;
    return p;
  }
  return NULL;
}



void pilha_apagar(PILHA **p) {
  if (p == NULL || *p == NULL) {
    return;
  }
   memset((*p)->historico, 0, sizeof((*p)->historico));
  (*p)->topo = -1;
}

bool pilha_vazia(PILHA *p) {
  if (p == NULL)
    return true;

  if (p->topo == -1) {
    return true;
  } else {
    return false;
  }
}

bool pilha_cheia(PILHA *p) {
    if(p == NULL) return true;

    if (p->topo == TAM - 1) {
        return true;
    } else {
        return false;
    }
}

int pilha_tamanho(PILHA *p) {
    if (p == NULL) return -1;

    return (p->topo)+1;
}

int pilha_topo(PILHA *p) {
    if (p == NULL) return 0;

    if (p->topo == -1) return 0;

    return p->topo;
}

bool pilha_empilhar(PILHA *p, char* medicamento) {
    if (p == NULL || medicamento != NULL) return false;

    if (pilha_cheia(p)) return false;

    p->topo++;
    strcpy(p->historico[p->topo], medicamento);
    return true;
}

PACIENTE *pilha_desempilhar(PILHA *p) {
    if (p == NULL) return NULL;

    if (pilha_vazia(p)) return NULL;

    PACIENTE *historico = p->historico[p->topo];
    memset(p->historico[p->topo], 0, 101);
    p->topo--;
    return historico;
}

void pilha_imprimir(PILHA *p) {
    if (p == NULL) return;

    for (int i = p->topo; i >= 0; i--) {
        printf("%s\n", p->historico[i]);
    }
}
