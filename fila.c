#include "fila.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RESET       "\033[0m"
#define YELLOW "\033[38;5;229m"  // Amarelo claro (atenção)
#define RED    "\033[38;5;203m"  // Vermelho pálido (erro / emergência)
#define CYAN   "\033[38;5;87m"   // Azul claro (info)
#define BLUE   "\033[38;5;75m"   // Azul calmo (padrão hospitalar)

struct fila {
    PACIENTE *pacientes[TAM_FILA];
    int inicio;
    int fim;
    int tamanho;
};

FILA *fila_criar() {
    FILA *f = (FILA *)malloc(sizeof(FILA));
    if (f != NULL) {
        f->inicio = 0;
        f->fim = 0;
        f->tamanho = 0;
    }
    return f;
}

void fila_apagar(FILA **f) {
    if (f == NULL || *f == NULL) {
        return;
    }
    free(*f);
}

bool fila_vazia(FILA *f) {
    if (f == NULL) return true;

    if (f->tamanho == 0) return true;
    else return false;
}

bool fila_cheia(FILA *f) {
    if (f == NULL) return true;
    if (f->tamanho == TAM_FILA) return true;
    else return false;
}

int fila_tamanho(FILA *f) {
    if (f == NULL) return -1;
    return f->tamanho;
}

PACIENTE *fila_inicio(FILA *f) {
    if (f == NULL) return NULL;
    return f->pacientes[f->inicio];
}

bool fila_inserir(FILA *f, PACIENTE *paciente) {
    if (f == NULL || paciente == NULL) return false;
    if (fila_cheia(f)){
        printf(RED"ERRO: Fila de espera cheia, não é possível inserir novo paciente.\n"RESET);
        return false;
    } 
    f->pacientes[f->fim] = paciente;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->tamanho++;
    return true;
}

PACIENTE *fila_remover(FILA *f) {
    if (f == NULL) return NULL;
    if (fila_vazia(f)) return NULL;
    PACIENTE *paciente = f->pacientes[f->inicio];
    f->pacientes[f->inicio] = NULL;
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->tamanho--;
    return paciente;
}

void fila_imprimir(FILA *f) {
    if (f == NULL || fila_vazia(f)) {
        printf(YELLOW"Fila vazia.\n"RESET);
        return;
    }
    int count = 0;
    int i = f->inicio;
    printf(CYAN"           === Fila de Espera ===\n\n"RESET);
    while (count < f->tamanho) {
        paciente_imprimir(f->pacientes[i]);
        i = (i + 1) % TAM_FILA;
        count++;
    }
    printf(CYAN"\nTotal na fila: %d\n"RESET, f->tamanho);
}

bool fila_contem_paciente(FILA *f, int id) {
    if (f == NULL || fila_vazia(f)) return false;
    
    int count = 0;
    int i = f->inicio;
    while (count < f->tamanho) {
        if (paciente_get_id(f->pacientes[i]) == id) {
            return true;
        }
        i = (i + 1) % TAM_FILA;
        count++;
    }
    return false;
}

bool fila_salvar_json(FILA *f, const char *filename) {
    if (f == NULL || filename == NULL) return false;
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf(RED"ERRO: Não foi possível abrir o arquivo %s para escrita.\n"RESET, filename);
        return false;
    }
    
    fprintf(file, "{\n");
    fprintf(file, "  \"fila_espera\": [\n");
    
    if (!fila_vazia(f)) {
        int count = 0;
        int i = f->inicio;
        while (count < f->tamanho) {
            fprintf(file, "    %d", paciente_get_id(f->pacientes[i]));
            if (count < f->tamanho - 1) {
                fprintf(file, ",\n");
            } else {
                fprintf(file, "\n");
            }
            i = (i + 1) % TAM_FILA;
            count++;
        }
    }
    
    fprintf(file, "  ]\n");
    fprintf(file, "}\n");
    
    fclose(file);
    return true;
}

bool fila_carregar_json(FILA *f, void *lista_void, const char *filename) {
    if (f == NULL || lista_void == NULL || filename == NULL) return false;
    
    LISTA *lista = (LISTA*)lista_void;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        // Arquivo não existe ainda, não é erro
        return false;
    }
    
    char buffer[200];
    int pacientes_carregados = 0;
    
    // Procurar pelo array da fila
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, "\"fila_espera\"")) {
            // Ler IDs
            while (fgets(buffer, sizeof(buffer), file)) {
                // Verificar fim do array
                if (strstr(buffer, "]")) {
                    break;
                }
                
                // Ler ID
                int id;
                if (sscanf(buffer, "%d", &id) == 1) {
                    // Buscar paciente na lista
                    PACIENTE* p = lista_buscar_paciente(lista, id);
                    if (p != NULL) {
                        if (fila_inserir(f, p)) {
                            pacientes_carregados++;
                        }
                    }
                }
            }
            break;
        }
    }
    
    fclose(file);
    printf(BLUE"Carregados %d paciente(s) do arquivo %s\n", pacientes_carregados, filename);
    return pacientes_carregados > 0;
}
