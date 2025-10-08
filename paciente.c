#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "paciente.h"

#define RESET       "\033[0m"    //Reseta a cor
#define WHITE  "\033[38;5;255m"  // Branco (limpeza / fundo)
// Alertas e status
#define YELLOW "\033[38;5;229m"  // Amarelo claro (atenção)
#define RED    "\033[38;5;203m"  // Vermelho pálido (erro / emergência)
#define CYAN   "\033[38;5;87m"   // Azul claro (info)


typedef struct paciente PACIENTE;

struct paciente {
    int id;
    char nome[100];
    PILHA* historico;
};

PACIENTE* paciente_criar(int id, const char* nome) {
    PACIENTE* p = (PACIENTE*)malloc(sizeof(PACIENTE));
    if (p != NULL) {
        p->id = id;
        strncpy(p->nome, nome, sizeof(p->nome) - 1);
        strcat(p->nome, "\0");
        p->historico = pilha_criar();
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

int paciente_get_id(PACIENTE* p) {
    if (p != NULL) {
        return p->id;
    }
    return -1; // Indica erro
}

const char* paciente_get_name(PACIENTE* p) {
    if (p != NULL) {
        return p->nome;
    }
    return NULL; // Indica erro
}

bool paciente_add_medicamento(PACIENTE* p, char* medicamento) {
    if (p != NULL && medicamento != NULL) {
        bool success;
        success = pilha_empilhar(p->historico, medicamento);
        if (success) {
            return true;
        }
        return false;
    }
    return false;
}
bool paciente_remover_medicamento(PACIENTE* p) {
    if (p != NULL) {
        char* medicamento = pilha_desempilhar(p->historico);
        if (medicamento != NULL) {
            return true;
        }
        return false;
    }
    return false;
}

char* paciente_retirar_ultimo_medicamento(PACIENTE* p) {
    if (p != NULL) {
        return pilha_desempilhar(p->historico);
    }
    return NULL;
}

bool paciente_imprimir_historico(PACIENTE* p) {
    if (p != NULL) {
        pilha_imprimir(p->historico);
        return true;
    }
    return false;
}

void paciente_imprimir(PACIENTE* p) {
    if (p != NULL) {
        printf(WHITE" %d - %s\n"RESET, p->id, p->nome);
    }
}

PILHA* paciente_get_historico(PACIENTE* p) {
    if (p != NULL) {
        return p->historico;
    }
    return NULL;
}

void paciente_salvar_json(PACIENTE* p, FILE *file) {
    if (p == NULL || file == NULL) return;
    
    fprintf(file, "    {\n");
    fprintf(file, "      \"id\": %d,\n", p->id);
    fprintf(file, "      \"nome\": \"%s\",\n", p->nome);
    fprintf(file, "      ");
    pilha_salvar_json(p->historico, file);
    fprintf(file, "\n");
    fprintf(file, "    }");
}

PACIENTE* paciente_carregar_json(FILE *file) {
    if (file == NULL) return NULL;
    
    char buffer[200];
    int id = -1;
    char nome[100] = "";
    
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