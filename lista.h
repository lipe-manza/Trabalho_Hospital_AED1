#ifndef LISTA_H
	#define LISTA_H

	#include "paciente.h"

	typedef struct lista LISTA;
    typedef struct no NO;

	LISTA *lista_criar();
	bool lista_inserir_paciente(LISTA *lista, PACIENTE *item);
	PACIENTE *lista_remover_paciente(LISTA *lista, int id);
	PACIENTE *lista_buscar_paciente(LISTA *lista, int id);
	bool lista_apagar(LISTA **lista);
	bool lista_vazia(LISTA *lista);
	bool lista_cheia(LISTA *lista);
	void lista_imprimir_pacientes(LISTA *lista);
	bool lista_salvar_json(LISTA *lista, const char *filename);
	bool lista_carregar_json(LISTA *lista, const char *filename);

#endif
