#include "pilha.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Resetar a cor
#define RESET       "\033[0m"    
#define WHITE  "\033[38;5;255m"  
// Alertas e status
#define YELLOW "\033[38;5;229m"  
#define RED    "\033[38;5;203m"  
#define CYAN   "\033[38;5;87m"   

// Pilha sequencial 
struct pilha {
    char historico[TAM_PILHA][101]; // Array de strings ou matrix de chars com 10 strings de tamanho 100 cada uma
    int topo; // Topo da pilha
};

// Função que cria a pilha
PILHA* pilha_criar() {
    PILHA* p = (PILHA*)malloc(sizeof(PILHA));
    if (p != NULL) {
        p->topo = -1; // Seta o topo para -1 para indicar que ta vazia
        return p;
    }
    return NULL;
}



void pilha_apagar(PILHA** p) {
    if (p == NULL || *p == NULL) {
        return;
    }
    memset((*p)->historico, 0, sizeof((*p)->historico));
    (*p)->topo = -1;
}

bool pilha_vazia(PILHA* p) {
    if (p == NULL)
        return true;

    if (p->topo == -1) { // Verifica se o topo é -1 ou seja se esta vazia 
        return true;
    }
    else {
        return false;
    }
}

bool pilha_cheia(PILHA* p) {
    if (p == NULL) return true;

    if (p->topo == TAM_PILHA - 1) { // Verifica se o topo tem indice maximo
        return true;
    }
    else {
        return false;
    }
}

int pilha_tamanho(PILHA* p) {
    if (p == NULL) return -1;

    return (p->topo) + 1;
}

int pilha_topo(PILHA* p) {
    if (p == NULL) return 0;

    if (p->topo == -1) return 0;

    return p->topo;
}

// Empilha o procedimento medico na pilha
bool pilha_empilhar(PILHA* p, char* medicamento) {
    if (p == NULL || medicamento == NULL) return false;

    if (pilha_cheia(p)) return false;

    p->topo++;
    strcpy(p->historico[p->topo], medicamento); // Adiciona o procedimento no topo da pilha
    return true;
}

// Desempilha o procedimento medico na pilha e retorna o procedimento
char* pilha_desempilhar(PILHA* p) {
    if (p == NULL) return NULL;

    if (pilha_vazia(p)) return NULL;

    static char historico[101];
    strcpy(historico, p->historico[p->topo]);
    memset(p->historico[p->topo], 0, 101);
    p->topo--;
    return historico;
}

// Imprime a pilha
void pilha_imprimir(PILHA* p) {
    if (p == NULL || pilha_vazia(p)) {
        printf(YELLOW"Histórico vazio.\n"RESET);
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf(WHITE"%d°- %s\n"RESET, i + 1, p->historico[i]);
    }
}

// Salva o histórico da pilha em formato JSON no arquivo fornecido  
void pilha_salvar_json(PILHA* p, FILE* file) {
    if (p == NULL || file == NULL) return;

    fprintf(file, "\"historico\": [");

    // Salvar do topo para a base (ordem first in last out)
    for (int i = 0; i <= p->topo; i++) {
        fprintf(file, "\"%s\"", p->historico[i]);
        if (i < p->topo) {
            fprintf(file, ", ");
        }
    }

    fprintf(file, "]");
}

// Carrega o histórico de uma pilha a partir de um arquivo JSON
bool pilha_carregar_json(PILHA* p, FILE* file) {
    if (p == NULL || file == NULL) return false;

    char buffer[200];
    char procedimento[101];
    int count = 0;

    // Ler até encontrar o array de histórico
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, "\"historico\"")) {
            // Procurar pelo '['
            char* start = strchr(buffer, '[');
            if (start == NULL) {
                if (fgets(buffer, sizeof(buffer), file) == NULL) return false;
                start = strchr(buffer, '[');
            }
            if (start == NULL) return false;

            start++;

            // Ler procedimentos
            while (*start != ']' && count < TAM_PILHA) {
                // Pular espaços
                while (*start == ' ' || *start == '\n' || *start == '\t') start++;

                if (*start == ']') break;

                // Ler string entre aspas
                if (*start == '"') {
                    start++;
                    int i = 0;
                    while (*start != '"' && *start != '\0' && i < 100) {
                        procedimento[i++] = *start++;
                    }
                    procedimento[i] = '\0';

                    // Empilhar (mas precisamos inverter a ordem para manter LIFO)
                    if (strlen(procedimento) > 0) {
                        strcpy(p->historico[count], procedimento);
                        count++;
                    }

                    if (*start == '"') start++;
                }

                // Pular vírgula
                while (*start == ',' || *start == ' ' || *start == '\n' || *start == '\t') {
                    start++;
                    if (*start == '\0') {
                        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
                        start = buffer;
                    }
                }
            }

            // Ajustar topo (invertemos a ordem ao carregar)
            p->topo = count - 1;
            return true;
        }
    }

    return false;
}
