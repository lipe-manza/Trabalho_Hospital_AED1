#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "paciente.h"
#include "fila.h"
#include "lista.h"


int display_menu() {
    int choice;
    printf("Menu:\n");
    printf("1. Registrar Paciente\n");
    printf("2. Registrar óbito do paciente\n");
    printf("3. Adicionar procedimento ao histórico médico\n");
    printf("4. Desfazer procedimento do histórico médico\n");
    printf("5. Chamar paciente para atendimento\n");
    printf("6. Mostrar fila de espera\n");
    printf("7. Mostrar histórico médico do paciente\n");
    printf("8. Sair\n");
    printf("Escolha uma opção: ");
    if (scanf("%d", &choice) != 1) {
        printf("Entrada inválida. Tente novamente.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    return choice;
}



int main() {
    int choice;

    FILA* fila_de_espera = fila_criar();
    LISTA* lista_de_pacientes = lista_criar();

    do {
        choice = display_menu();

        switch (choice)
        {
        case 1:
            printf("Insira o nome do paciente: ");
            char nome[100];
            int id;
            scanf("%d %s", id, nome);
            PACIENTE* novo_paciente = paciente_criar(id, nome);
            fila_inserir(fila_de_espera, novo_paciente);
            lista_inserir(lista_de_pacientes, novo_paciente);
            printf("Paciente %s registrado com ID %d e adicionado à fila de espera.\n", nome, paciente_get_id(novo_paciente));
            break;
        
        case 2:
            printf("Insira o ID do paciente que faleceu: ");
            int id_obito;
            scanf("%d", &id_obito);
            lista_remover(lista_de_pacientes, id_obito);
            printf("Paciente com ID %d removido da lista de pacientes.\n", id_obito);
            break;
        case 3:
            printf("Insira o ID do paciente e o procedimento a ser adicionado: ");
            int id_proc;
            char procedimento[100];
            scanf("%d %s", &id_proc, procedimento);
            PACIENTE* paciente_proc = lista_busca(lista_de_pacientes, id_proc);
            if (paciente_proc != NULL) {
                paciente_add_historico(paciente_proc, procedimento);
                printf("Procedimento '%s' adicionado ao histórico do paciente ID %d.\n", procedimento, id_proc);
            } else {
                printf("Paciente com ID %d não encontrado.\n", id_proc);
            }
            break;
        case 4:
            printf("Insira o ID do paciente para desfazer o último procedimento: ");
            int id_desfazer;
            scanf("%d", &id_desfazer);
            PACIENTE* paciente_desfazer = lista_busca(lista_de_pacientes, id_desfazer);
            if (paciente_desfazer != NULL) {
                if (paciente_remove_medicamento(paciente_desfazer)) {
                    printf("Último procedimento removido do histórico do paciente ID %d.\n", id_desfazer);
                } else {
                    printf("Nenhum procedimento para remover no histórico do paciente ID %d.\n", id_desfazer);
                }
            } else {
                printf("Paciente com ID %d não encontrado.\n", id_desfazer);
            }
            break;
        case 5:
            PACIENTE* paciente_atendimento = fila_remover(fila_de_espera);
            if (paciente_atendimento != NULL) {
                printf("Paciente %s (ID %d) chamado para atendimento.\n", paciente_get_name(paciente_atendimento), paciente_get_id(paciente_atendimento));
            } else {
                printf("Nenhum paciente na fila de espera.\n");
            }
            break;
        case 6:
            printf("Fila de espera:\n");
            fila_imprimir(fila_de_espera);
            break;
        case 7:
            printf("Insira o ID do paciente para ver o histórico médico: ");
            int id_historico;
            scanf("%d", &id_historico);
            PACIENTE* paciente_historico = lista_busca(lista_de_pacientes, id_historico);
            if (paciente_historico != NULL) {        
                printf("Histórico médico do paciente %s (ID %d):\n", paciente_get_name(paciente_historico), paciente_get_id(paciente_historico));
                paciente_imprimir_historico(paciente_historico);
            } else {
                printf("Paciente com ID %d não encontrado.\n", id_historico);
            }
            break;
        default:
            break;
        }
    } while (choice != 8);
    return 0;
}