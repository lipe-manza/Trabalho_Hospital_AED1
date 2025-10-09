#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "paciente.h"

// Reseta a cor
#define RESET       "\033[0m"

#define WHITE  "\033[38;5;255m"  
// Alertas e status
#define YELLOW "\033[38;5;229m"  
#define RED    "\033[38;5;203m"  
#define CYAN   "\033[38;5;87m"   

// Struct paciente que armazena o id nome e um historico de cada paciente
struct paciente {
    int id;
    char nome[100];
    PILHA* historico; // Ponteiro para a pilha que armazena o histórico do paciente
};

// Função que cria o paciente com seu id e nome
PACIENTE* paciente_criar(int id, const char* nome) {
    PACIENTE* p = (PACIENTE*)malloc(sizeof(PACIENTE));
    if (p != NULL) {
        p->id = id;
        strncpy(p->nome, nome, sizeof(p->nome) - 1);
        strcat(p->nome, "\0");
        p->historico = pilha_criar(); // Cria o historico do paciente
        if (p->historico != NULL) {
            return p;
        }
        return NULL;
    }
    return NULL;
}

bool paciente_apagar(PACIENTE** p) {
    if (p != NULL && *p != NULL) {
        pilha_apagar(&(*p)->historico);
        free(*p);
        *p = NULL;
        return true;
    }
    return false;
}

// Função para retornar o id do paciente
int paciente_get_id(PACIENTE* p) {
    if (p != NULL) {
        return p->id;
    }
    return -1; // Indica erro
}

// Função para retornar o nome do paciente
const char* paciente_get_name(PACIENTE* p) {
    if (p != NULL) {
        return p->nome;
    }
    return NULL; // Indica erro
}

// Função para adicionar um medicamento/procedimento no historico do paciente(pilha)
bool paciente_add_medicamento(PACIENTE* p, char* medicamento) {
    if (p != NULL && medicamento != NULL) {
        bool success;
        success = pilha_empilhar(p->historico, medicamento); // Empilha o procedimento no historico do paciente
        if (success) {
            return true;
        }
        return false;
    }
    return false;
}

// Função para remover um medicamento/procedimento no historico do paciente(pilha) e não retorna nada
bool paciente_remover_medicamento(PACIENTE* p) {
    if (p != NULL) {
        char* medicamento = pilha_desempilhar(p->historico); //Desempilha o procedimento no historico do paciente
        if (medicamento != NULL) {
            return true;
        }
        return false;
    }
    return false;
}

// Função para remover um medicamento/procedimento no historico do paciente(pilha) e retorna o procedimento
char* paciente_retirar_ultimo_medicamento(PACIENTE* p) {
    if (p != NULL) {
        return pilha_desempilhar(p->historico);
    }
    return NULL;
}

// Imprime o histórico médico do paciente 
bool paciente_imprimir_historico(PACIENTE* p) {
    if (p != NULL) {
        pilha_imprimir(p->historico); // Chama a função para imprimir a pilha
        return true;
    }
    return false;
}

// Função para imprimir o ID e o nome do paciente 
void paciente_imprimir(PACIENTE* p) {
    if (p != NULL) {
        printf(WHITE" %d - %s\n"RESET, p->id, p->nome);
    }
}

// Função que retorna o historico do paciente
PILHA* paciente_get_historico(PACIENTE* p) {
    if (p != NULL) {
        return p->historico;
    }
    return NULL;
}

// Salva os dados de um paciente em formato JSON no arquivo fornecido.
void paciente_salvar_json(PACIENTE* p, FILE *file) {
    if (p == NULL || file == NULL) return;
    // - Escreve o objeto JSON com:
    //     - "id": o identificador do paciente.
    //     - "nome": o nome do paciente.
    //     - "historico": chama pilha_salvar_json para salvar o histórico do paciente.
    fprintf(file, "    {\n");
    fprintf(file, "      \"id\": %d,\n", p->id);
    fprintf(file, "      \"nome\": \"%s\",\n", p->nome);
    fprintf(file, "      ");
    pilha_salvar_json(p->historico, file);
    fprintf(file, "\n");
    fprintf(file, "    }");

    // - Não fecha o arquivo; isso deve ser feito pelo chamador.
}

// Carrega os dados de um paciente de um arquivo JSON
PACIENTE* paciente_carregar_json(FILE *file) {
    if (file == NULL) return NULL;
    
    char buffer[200];
    int id = -1;
    char nome[100] = "";
    
    // Lê linha por linha procurando os campos "id", "nome" e "historico".
    // Ler ID e nome
    while (fgets(buffer, sizeof(buffer), file)) {
        // Procurar ID
        if (strstr(buffer, "\"id\"")) {
            char *start = strchr(buffer, ':');
            if (start) {
                sscanf(start + 1, "%d", &id);
            }
        }
        // Procurar nome
        else if (strstr(buffer, "\"nome\"")) {
            char *start = strchr(buffer, '"');
            if (start) {
                start = strchr(start + 1, '"'); // Pular "nome"
                if (start) {
                    start = strchr(start + 1, '"'); // Encontrar início do valor
                    if (start) {
                        start++;
                        int i = 0;
                        while (*start != '"' && *start != '\0' && i < 99) {
                            nome[i++] = *start++;
                        }
                        nome[i] = '\0';
                    }
                }
            }
        }
        // Procurar histórico
        else if (strstr(buffer, "\"historico\"")) {
            // Criar paciente com ID e nome
            if (id >= 0 && strlen(nome) > 0) {
                PACIENTE* p = paciente_criar(id, nome);
                if (p != NULL) {
                    // Voltar o ponteiro do arquivo para ler o histórico
                    fseek(file, -strlen(buffer), SEEK_CUR);
                    pilha_carregar_json(p->historico, file);
                    return p;
                }
            }
            return NULL;
        }
    }
    
    // Se chegou aqui, criar paciente sem histórico
    if (id >= 0 && strlen(nome) > 0) {
        return paciente_criar(id, nome);
    }
    
    return NULL;
}