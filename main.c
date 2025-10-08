#include <stdio.h>
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
        {
            printf("Insira o ID e nome do paciente: ");
            char nome[100];
            int id;
            scanf("%d %s", &id, nome);
            
            // Verificar se ID já existe
            if (lista_buscar_paciente(lista_de_pacientes, id) != NULL) {
                printf("ERRO: Paciente com ID %d já está registrado. IDs duplicados não são permitidos.\n", id);
                break;
            }
            
            // Verificar se a fila está cheia
            if (fila_cheia(fila_de_espera)) {
                printf("ERRO: Fila de espera está cheia. Não é possível registrar mais pacientes no momento.\n");
                break;
            }
            
            PACIENTE* novo_paciente = paciente_criar(id, nome);
            if (novo_paciente == NULL) {
                printf("ERRO: Não foi possível criar o paciente.\n");
                break;
            }
            
            if (!lista_inserir_paciente(lista_de_pacientes, novo_paciente)) {
                printf("ERRO: Não foi possível inserir o paciente na lista de pacientes.\n");
                paciente_apagar(&novo_paciente);
                break;
            }
            
            if (!fila_inserir(fila_de_espera, novo_paciente)) {
                printf("ERRO: Não foi possível adicionar o paciente à fila de espera.\n");
                lista_remover_paciente(lista_de_pacientes, id);
                paciente_apagar(&novo_paciente);
                break;
            }
            
            printf("Paciente %s registrado com ID %d e adicionado à fila de espera.\n", nome, id);
            break;
        }
        
        case 2:
        {
            printf("Insira o ID do paciente que faleceu: ");
            int id_obito;
            scanf("%d", &id_obito);
            
            // Verificar se o paciente existe
            PACIENTE* paciente_obito = lista_buscar_paciente(lista_de_pacientes, id_obito);
            if (paciente_obito == NULL) {
                printf("ERRO: Paciente com ID %d não encontrado no sistema.\n", id_obito);
                break;
            }
            
            // Verificar se o paciente ainda está na fila de espera
            if (fila_contem_paciente(fila_de_espera, id_obito)) {
                printf("ERRO: Paciente com ID %d ainda está na fila de espera. Pacientes na fila não podem falecer!\n", id_obito);
                break;
            }
            
            // Remover paciente da lista e liberar memória
            PACIENTE* removido = lista_remover_paciente(lista_de_pacientes, id_obito);
            if (removido != NULL) {
                printf("Paciente %s (ID %d) removido da lista de pacientes devido a óbito.\n", 
                       paciente_get_name(removido), id_obito);
                paciente_apagar(&removido);
            } else {
                printf("ERRO: Não foi possível remover o paciente com ID %d.\n", id_obito);
            }
            break;
        }
        case 3:
        {
            printf("Insira o ID do paciente e o procedimento a ser adicionado: ");
            int id_proc;
            char procedimento[100];
            scanf("%d %s", &id_proc, procedimento);
            PACIENTE* paciente_proc = lista_buscar_paciente(lista_de_pacientes, id_proc);
            if (paciente_proc != NULL) {
                if (paciente_add_medicamento(paciente_proc, procedimento)) {
                    printf("Procedimento '%s' adicionado ao histórico do paciente %s (ID %d).\n", 
                           procedimento, paciente_get_name(paciente_proc), id_proc);
                } else {
                    printf("ERRO: Não foi possível adicionar o procedimento. Histórico pode estar cheio.\n");
                }
            } else {
                printf("ERRO: Paciente com ID %d não encontrado.\n", id_proc);
            }
            break;
        }
        case 4:
        {
            printf("Insira o ID do paciente para desfazer o último procedimento: ");
            int id_desfazer;
            scanf("%d", &id_desfazer);
            PACIENTE* paciente_desfazer = lista_buscar_paciente(lista_de_pacientes, id_desfazer);
            if (paciente_desfazer != NULL) {
                char* procedimento_removido = paciente_retirar_ultimo_medicamento(paciente_desfazer);
                if (procedimento_removido != NULL) {
                    printf("Procedimento '%s' foi removido do histórico do paciente %s (ID %d).\n", 
                           procedimento_removido, paciente_get_name(paciente_desfazer), id_desfazer);
                } else {
                    printf("ERRO: Nenhum procedimento para remover no histórico do paciente %s (ID %d).\n",
                           paciente_get_name(paciente_desfazer), id_desfazer);
                }
            } else {
                printf("ERRO: Paciente com ID %d não encontrado.\n", id_desfazer);
            }
            break;
        }
        case 5:
        {
            PACIENTE* paciente_atendimento = fila_remover(fila_de_espera);
            if (paciente_atendimento != NULL) {
                printf("Paciente %s (ID %d) chamado para atendimento.\n", 
                       paciente_get_name(paciente_atendimento), 
                       paciente_get_id(paciente_atendimento));
            } else {
                printf("ERRO: Nenhum paciente na fila de espera.\n");
            }
            break;
        }
        case 6:
            fila_imprimir(fila_de_espera);
            break;
        case 7:
        {
            printf("Insira o ID do paciente para ver o histórico médico: ");
            int id_historico;
            scanf("%d", &id_historico);
            PACIENTE* paciente_historico = lista_buscar_paciente(lista_de_pacientes, id_historico);
            if (paciente_historico != NULL) {        
                printf("=== Histórico médico do paciente %s (ID %d) ===\n", 
                       paciente_get_name(paciente_historico), 
                       paciente_get_id(paciente_historico));
                paciente_imprimir_historico(paciente_historico);
            } else {
                printf("ERRO: Paciente com ID %d não encontrado.\n", id_historico);
            }
            break;
        }
        case 8:
            printf("Encerrando o sistema...\n");
            break;
        default:
            printf("Opção inválida! Por favor, escolha uma opção entre 1 e 8.\n");
            break;
        }
    } while (choice != 8);
    
    // Limpar memória antes de sair
    fila_apagar(&fila_de_espera);
    lista_apagar(&lista_de_pacientes);
    
    printf("Sistema encerrado com sucesso.\n");
    return 0;
}