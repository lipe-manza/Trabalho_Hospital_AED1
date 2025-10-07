#ifndef LISTA_H
	#define LISTA_H

	#include "paciente.h"

	typedef struct lista LISTA;
    typedef struct no NO;

	LISTA *lista_criar();
	bool lista_inserir(LISTA *lista, PACIENTE *item);
	PACIENTE *lista_remover(LISTA *lista, int id);
	bool lista_apagar(LISTA **lista);
	PACIENTE *lista_busca(LISTA *lista, int id);
	int lista_tamanho(LISTA *lista);
	bool lista_vazia(LISTA *lista);
	bool lista_cheia(LISTA *lista);
	void lista_imprimir_pacientes(LISTA *lista);



	/*Fun��es adicionais*/
	int lista_inverter(LISTA **lista);
	bool lista_comparar(LISTA *l1, LISTA *l2);

#endif
