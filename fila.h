#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#include "paciente.h"

#define TAM_FILA 100

typedef struct paciente PACIENTE;
typedef struct fila FILA;

FILA *fila_criar();
void fila_apagar(FILA **fila);
bool fila_vazia(FILA *fila);
bool fila_cheia(FILA *fila);
int fila_tamanho(FILA *fila);
PACIENTE *fila_inicio(FILA *fila);
bool fila_inserir(FILA *fila, PACIENTE *item);
PACIENTE *fila_remover(FILA *fila);
void fila_imprimir(FILA *fila);
bool fila_contem_paciente(FILA *fila, int id);
bool fila_salvar_json(FILA *fila, const char *filename);
bool fila_carregar_json(FILA *fila, void *lista, const char *filename);

#endif