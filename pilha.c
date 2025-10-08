#include "pilha.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct pilha {
  char historico[TAM_PILHA][101];
  int topo;
};

PILHA *pilha_criar() {
  PILHA *p = (PILHA *)malloc(sizeof(PILHA));
  if (p != NULL) {
    p->topo = -1;
    return p;
  }
  return NULL;
}



void pilha_apagar(PILHA **p) {
  if (p == NULL || *p == NULL) {
    return;
  }
   memset((*p)->historico, 0, sizeof((*p)->historico));
  (*p)->topo = -1;
}

bool pilha_vazia(PILHA *p) {
  if (p == NULL)
    return true;

  if (p->topo == -1) {
    return true;
  } else {
    return false;
  }
}

bool pilha_cheia(PILHA *p) {
    if(p == NULL) return true;

    if (p->topo == TAM_PILHA - 1) {
        return true;
    } else {
        return false;
    }
}

int pilha_tamanho(PILHA *p) {
    if (p == NULL) return -1;

    return (p->topo)+1;
}

int pilha_topo(PILHA *p) {
    if (p == NULL) return 0;

    if (p->topo == -1) return 0;

    return p->topo;
}

bool pilha_empilhar(PILHA *p, char* medicamento) {
    if (p == NULL || medicamento == NULL) return false;

    if (pilha_cheia(p)) return false;

    p->topo++;
    strcpy(p->historico[p->topo], medicamento);
    return true;
}

char* pilha_desempilhar(PILHA *p) {
    if (p == NULL) return NULL;

    if (pilha_vazia(p)) return NULL;

    static char historico[101];
    strcpy(historico, p->historico[p->topo]);
    memset(p->historico[p->topo], 0, 101);
    p->topo--;
    return historico;
}

void pilha_imprimir(PILHA *p) {
    if (p == NULL || pilha_vazia(p)) {
        printf("Histórico vazio.\n");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("- %s\n", p->historico[i]);
    }
}

void pilha_salvar_json(PILHA *p, FILE *file) {
    if (p == NULL || file == NULL) return;
    
    fprintf(file, "\"historico\": [");
    
    // Salvar do topo para a base (ordem first in last out)
    for (int i = p->topo; i >= 0; i--) {
        fprintf(file, "\"%s\"", p->historico[i]);
        if (i > 0) {
            fprintf(file, ", ");
        }
    }
    
    fprintf(file, "]");
}

bool pilha_carregar_json(PILHA *p, FILE *file) {
    if (p == NULL || file == NULL) return false;
    
    char buffer[200];
    char procedimento[101];
    int count = 0;
    
    // Ler até encontrar o array de histórico
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, "\"historico\"")) {
            // Procurar pelo '['
            char *start = strchr(buffer, '[');
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
