#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h" // Inclui TAD fila
#include "lista.h" // Inclui TAD lista

//Cores usadas nos prints para o terminal
#define RESET       "\033[0m"
#define YELLOW "\033[38;5;229m"  
#define RED    "\033[38;5;203m"  
#define CYAN   "\033[38;5;87m"   
#define BLUE   "\033[38;5;75m"   

struct fila { // Fila sequencial circular com tamanho limitado que forma a fila de triagem do hospital
    PACIENTE *pacientes[TAM_FILA]; // Array de ponteiros para structs PACIENTE
    int inicio;
    int fim;
    int tamanho;
};

// Cria a fila e aloca memória 
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

// Função para retornar o índice do final da fila 
PACIENTE *fila_inicio(FILA *f) {
    if (f == NULL) return NULL;
    return f->pacientes[f->inicio];
}

// Insere o paciente no final da fila
bool fila_inserir(FILA *f, PACIENTE *paciente) {
    if (f == NULL || paciente == NULL) return false;
    if (fila_cheia(f)){ // Verifica se a fila esta cheia e se estive cheia não poderá inserir
        printf(RED"ERRO: Fila de espera cheia, não é possível inserir novo paciente.\n"RESET);
        return false;
    } 
    f->pacientes[f->fim] = paciente; // Coloca o novo paciente no final da fila
    f->fim = (f->fim + 1) % TAM_FILA; // Coloca o fim para o novo fim usando mod já que é uma fila circular
    f->tamanho++;
    return true;
}

// Remove o paciente que está em primeiro na fila
PACIENTE *fila_remover(FILA *f) {
    if (f == NULL) return NULL;
    if (fila_vazia(f)) return NULL;
    PACIENTE *paciente = f->pacientes[f->inicio]; // Pega o paciente do inicio da fila
    f->pacientes[f->inicio] = NULL; // Faz o ponteiro pacientes no indices f-> inicio apontar para NULL
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
    while (count < f->tamanho) { // while para ir do começo da fila até o final com o delimitador(0->tamanho-1)
        paciente_imprimir(f->pacientes[i]);
        i = (i + 1) % TAM_FILA; // Vai para o próximo indice do array usando mod já que é circular
        count++;
    }
    printf(CYAN"\nTotal na fila: %d\n"RESET, f->tamanho);
}

// Função que verifica se já existe um paciente com o id, na fila
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

// Salva a fila de espera em um arquivo JSON antes do programa fechar.
bool fila_salvar_json(FILA *f, const char *filename) {
    if (f == NULL || filename == NULL) return false;
    
    FILE *file = fopen(filename, "w"); // Abre o arquivo em modo escrita.
    if (file == NULL) {
        printf(RED"ERRO: Não foi possível abrir o arquivo %s para escrita.\n"RESET, filename);
        return false;
    }
   
    fprintf(file, "{\n");
    fprintf(file, "  \"fila_espera\": [\n");
    
    if (!fila_vazia(f)) {
        int count = 0;
        int i = f->inicio;
        
        while (count < f->tamanho) { // Percorre a fila circular do início ao fim, imprimindo os IDs em ordem.
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
    
    fclose(file); // Fecha o arquivo
    return true;
}

// Carrega a fila de espera de um arquivo JSON ao iniciar o programa
bool fila_carregar_json(FILA *f, void *lista_void, const char *filename) {
    if (f == NULL || lista_void == NULL || filename == NULL) return false;
    
    LISTA *lista = (LISTA*)lista_void;
    
    FILE *file = fopen(filename, "r");// Abre o arquivo em modo leitura. 
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
    
    fclose(file); // Fecha o arquivo
    printf(BLUE"Carregados %d paciente(s) do arquivo %s\n", pacientes_carregados, filename);
    return pacientes_carregados > 0;
}
