#ifndef PILHA_H
#define PILHA_H

#include "paciente.h"
#include <stdbool.h>

#define TAM 10

typedef struct paciente ITEM;

typedef struct pilha PILHA;

PILHA *pilha_criar();
void pilha_apagar(PILHA **pilha);
bool pilha_vazia(PILHA *pilha);
bool pilha_cheia(PILHA *pilha);
int pilha_tamanho(PILHA *pilha);
ITEM *pilha_topo(PILHA *pilha);
bool pilha_empilhar(PILHA *pilha, ITEM *item);
ITEM *pilha_desempilhar(PILHA *pilha);



#endif