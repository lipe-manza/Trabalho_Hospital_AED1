# Projeto 1 — Pronto Socorro SUS  
**Disciplina:** SCC0202 - Algoritmos e Estruturas de Dados I  
**Professores:** Rudinei e JB  
**Monitores:** Laura e Felipe  

---

## Integrantes

| Nome completo | Número USP |
|----------------|-------------|
| Luiz Felipe Manzoli Franceschini | 16913300 |
| Gustavo  Vieira Gomes | 16907251 |
| Guilherme Pego dos Santos | 15575570 |

---

## Descrição do Projeto

O sistema simula o funcionamento de um **Pronto Socorro (PS)**, gerenciando pacientes, fila de atendimento e histórico médico.  
O projeto faz uso de **TADs (Tipos Abstratos de Dados)** — pilha, fila, lista e paciente — e realiza a **persistência dos dados em disco** usando o formato **JSON**.

---

## Estrutura do Sistema

- **Lista_de_Pacientes:** Armazena todos os pacientes registrados.  
- **Fila de Espera:** Gerencia a ordem de chegada dos pacientes.  
- **Histórico Médico (Pilha):** Registra os procedimentos de cada paciente.  
- **Paciente (Item):** Armazena os dados de cada paciente.  
- **Módulo de I/O:** Responsável por salvar e carregar os dados em disco.  

---

## Funcionalidades do Menu Principal

0. Mostrar lista de pacientes  
1. Registrar paciente  
2. Registrar óbito de paciente  
3. Adicionar procedimento ao histórico médico  
4. Desfazer procedimento do histórico médico  
5. Chamar paciente para atendimento  
6. Mostrar fila de espera  
7. Mostrar histórico do paciente  
8. Sair  

---

## Persistência de Dados

Todos os dados são salvos automaticamente ao sair do programa e recarregados ao iniciar.  
A persistência é feita em **arquivos `.json`** contendo:

- `lista_de_pacientes.json` — Lista de pacientes  
- `fila_da_triagem.json` — Fila de espera  
- `pacientes.json` — Históricos médicos  

---

## Compilação e Execução

A compilação e execução são feitas utilizando o **Makefile**:

```bash
make          # Compila o projeto
make all      # Compila o projeto (igual a make)
make clean    # Remove arquivos objeto e executável
make cleanall # Remove tudo, incluindo dados salvos
make run      # Compila e executa o programa
make rebuild  # Recompila tudo do zero
make check    # Verifica os arquivos fonte
make help     # Mostra esta mensagem de ajuda
```