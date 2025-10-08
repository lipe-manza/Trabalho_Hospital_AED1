#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>
#include <stdio.h>

#define TAM_PILHA 10

typedef struct pilha PILHA;

PILHA *pilha_criar();
void pilha_apagar(PILHA **pilha);
bool pilha_vazia(PILHA *pilha);
bool pilha_cheia(PILHA *pilha);
int pilha_tamanho(PILHA *pilha);
bool pilha_empilhar(PILHA *pilha, char* medicamento);
char* pilha_desempilhar(PILHA *pilha);
void pilha_imprimir(PILHA *pilha);
void pilha_salvar_json(PILHA *pilha, FILE *file);
bool pilha_carregar_json(PILHA *pilha, FILE *file);


#endif