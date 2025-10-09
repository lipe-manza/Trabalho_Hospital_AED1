#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "paciente.h"
#include "fila.h"
#include "lista.h"
#define RESET       "\033[0m"
#define BOLD        "\033[1m"

// Tons principais
#define BLUE   "\033[38;5;75m"   // Azul calmo (padrão hospitalar)
#define GREEN  "\033[38;5;78m"   // Verde suave (saúde)
#define WHITE  "\033[38;5;255m"  // Branco (limpeza / fundo)

// Alertas e status
#define YELLOW "\033[38;5;229m"  // Amarelo claro (atenção)
#define RED    "\033[38;5;203m"  // Vermelho pálido (erro / emergência)
#define CYAN   "\033[38;5;87m"   // Azul claro (info)







int display_menu() {
    int choice;
    printf("\n");
    printf(WHITE"==============================================\n");
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
    if (scanf("%d", &choice) != 1) { // verifica se a entrada é válida
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    printf("\n==============================================\n"RESET);
    printf("\n");
    return choice;
}



int main() {
    int choice;

    FILA* fila_de_espera = fila_criar();
    LISTA* lista_de_pacientes = lista_criar();

    // Carregar dados salvos
    printf(WHITE"===========================================\n"RESET);
    printf(GREEN"   Sistema de Gestão Hospitalar - PS\n"RESET);
    printf(WHITE"===========================================\n\n"RESET);
    printf(BLUE"Carregando dados salvos...\n");
    lista_carregar_json(lista_de_pacientes, "Lista_de_pacientes.json");
    fila_carregar_json(fila_de_espera, lista_de_pacientes, "Fila_da_triagem.json");
    printf("\n");


    //? inicio do menu e dos switchs
    do {
        choice = display_menu();

        switch (choice)
        {
        case 1:
        {
            int id;
            char nome[100];

            printf("Insira o ID do paciente: ");
            scanf("%d", &id);
            while (id <= 0) {// verifica se o ID é positivo
                printf(RED"ERRO: ID inválido. Por favor, insira um ID positivo.\n"RESET);
                printf("Insira o ID do paciente: ");
                scanf("%d", &id);
            }
            
            // Verificar se ID já existe
            PACIENTE* paciente_antigo = lista_buscar_paciente(lista_de_pacientes, id);
            if (paciente_antigo != NULL) {
                if (fila_contem_paciente(fila_de_espera, id)) {
                    printf(RED"ERRO: Paciente com ID %d já está registrado e já está na fila de espera.\n"RESET, id);
                    break;
                }
                else {
                    fila_inserir(fila_de_espera, paciente_antigo);// insere o paciente ja registrado na fila
                    printf(RED"ERRO: Paciente com ID %d já está registrado.\n"RESET, id);
                    printf(CYAN"Paciente %s ID(%d) adicionado a fila de espera"RESET, paciente_get_name(paciente_antigo), id);
                    break;
                }
            }
            printf("Insira o nome do paciente:");
            scanf(" %[^\n]", nome);



            // Verificar se a fila está cheia
            if (fila_cheia(fila_de_espera)) {
                printf(RED"ERRO: Fila de espera está cheia. Não é possível registrar mais pacientes no momento.\n"RESET);
                break;
            }

            PACIENTE* novo_paciente = paciente_criar(id, nome);
            if (novo_paciente == NULL) {
                printf(RED"ERRO: Não foi possível criar o paciente.\n"RESET);
                break;
            }

            if (!lista_inserir_paciente(lista_de_pacientes, novo_paciente)) {
                printf(RED"ERRO: Não foi possível inserir o paciente na lista de pacientes.\n"RESET);
                paciente_apagar(&novo_paciente);
            }

            if (!fila_inserir(fila_de_espera, novo_paciente)) {
                printf(RED"ERRO: Não foi possível adicionar o paciente à fila de espera.\n"RESET);
                lista_remover_paciente(lista_de_pacientes, id);
                paciente_apagar(&novo_paciente);
                break;
            }


            printf(CYAN "\nPaciente %s registrado com ID %d e adicionado à fila de espera.\n" RESET, nome, id);

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
                printf(RED"ERRO: Paciente com ID %d não encontrado no sistema.\n"RESET, id_obito);
                break;
            }

            // Verificar se o paciente ainda está na fila de espera
            if (fila_contem_paciente(fila_de_espera, id_obito)) {
                printf(RED"ERRO: Paciente com ID %d ainda está na fila de espera. Pacientes na fila não podem falecer!\n"RESET, id_obito);
                break;
            }

            // Remover paciente da lista e liberar memória
            PACIENTE* removido = lista_remover_paciente(lista_de_pacientes, id_obito);
            if (removido != NULL) {
                printf(YELLOW"Paciente %s (ID %d) removido da lista de pacientes devido a óbito.\n"RESET,
                    paciente_get_name(removido), id_obito);
                paciente_apagar(&removido);
            }
            else {
                printf(RED"ERRO: Não foi possível remover o paciente com ID %d.\n"RESET, id_obito);
            }
            break;
        }
        case 3:
        {
            int id_proc;
            char procedimento[100];
            printf("Insira o ID do paciente:");
            scanf(" %d", &id_proc);
            PACIENTE* paciente_proc = lista_buscar_paciente(lista_de_pacientes, id_proc);
            if (paciente_proc != NULL) {
                printf("Insira o procedimento a ser adicionado ao histórico médico de %s: ", paciente_get_name(lista_buscar_paciente(lista_de_pacientes, id_proc)));
                scanf(" %[^\n]", procedimento);
                if (paciente_add_medicamento(paciente_proc, procedimento)) {
                    printf(CYAN"\nProcedimento '%s' adicionado ao histórico do paciente %s (ID %d).\n"CYAN,
                        procedimento, paciente_get_name(paciente_proc), id_proc);
                }
                else {
                    printf(RED"ERRO: Não foi possível adicionar o procedimento. Histórico pode estar cheio.\n"RESET);
                }
            }
            else {
                printf(RED"ERRO: Paciente com ID %d não encontrado.\n"RESET, id_proc);
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
                    printf(CYAN"Procedimento '%s' foi removido do histórico do paciente %s (ID %d).\n"RESET,
                        procedimento_removido, paciente_get_name(paciente_desfazer), id_desfazer);
                }
                else {
                    printf(RED"ERRO: Nenhum procedimento para remover no histórico do paciente %s (ID %d).\n"RESET,
                        paciente_get_name(paciente_desfazer), id_desfazer);
                }
            }
            else {
                printf(RED"ERRO: Paciente com ID %d não encontrado.\n"RESET, id_desfazer);
            }
            break;
        }
        case 5:
        {
            PACIENTE* paciente_atendimento = fila_remover(fila_de_espera);
            if (paciente_atendimento != NULL) {
                printf(CYAN"Paciente %s (ID %d) chamado para atendimento.\n"RESET,
                    paciente_get_name(paciente_atendimento),
                    paciente_get_id(paciente_atendimento));
            }
            else {
                printf(RED"ERRO: Nenhum paciente na fila de espera.\n"RESET);
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
                printf(CYAN"\n=== Histórico médico do paciente %s (ID %d) ===\n\n"RESET, paciente_get_name(paciente_historico), paciente_get_id(paciente_historico));

                paciente_imprimir_historico(paciente_historico);
            }
            else {
                printf(RED"ERRO: Paciente com ID %d não encontrado.\n"RESET, id_historico);
            }
            break;
        }
        case 8:
            printf(BLUE"Encerrando o sistema...\n"RESET);
            break;
        default:
            printf(RED"Opção inválida! Por favor, escolha uma opção entre 1 e 8.\n"RESET);
            break;
        }
    } while (choice != 8);

    // Salvar dados antes de sair
    printf(BLUE"\nSalvando dados...\n"RESET);
    lista_salvar_json(lista_de_pacientes, "Lista_de_pacientes.json");
    fila_salvar_json(fila_de_espera, "Fila_da_triagem.json");
    printf(GREEN"\nDados salvos com sucesso!\n\n"RESET);

    // Limpar memória antes de sair
    fila_apagar(&fila_de_espera);
    lista_apagar(&lista_de_pacientes);

    printf(GREEN"Sistema encerrado com sucesso.\n"RESET);
    return 0;
}