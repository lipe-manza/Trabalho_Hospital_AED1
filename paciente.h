#ifndef PACIENTE_H
#define PACIENTE_H

#include <stdbool.h>
#include "pilha.h"

typedef struct paciente PACIENTE;

PACIENTE* paciente_criar(int id, const char* name);
bool paciente_apagar(PACIENTE** paciente);
void paciente_imprimir(PACIENTE* paciente);
int paciente_get_id(PACIENTE* paciente);
const char* paciente_get_name(PACIENTE* paciente);
bool paciente_add_historico(PACIENTE* paciente, const char* medicamento);
bool paciente_imprimir_historico(PACIENTE* paciente);
bool paciente_add_medicamento(PACIENTE* paciente, char* medicamento);
bool paciente_remover_medicamento(PACIENTE* paciente);
char* paciente_retirar_ultimo_medicamento(PACIENTE* paciente);

#endif