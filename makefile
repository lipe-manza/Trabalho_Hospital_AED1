# Makefile para Sistema de Gestão Hospitalar

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c99 -g

# Nome do executável
TARGET = hospital

# Arquivos objeto
OBJS = main.o paciente.o fila.o lista.o pilha.o

# Regra principal
all: $(TARGET)

# Compilar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Compilação concluída! Execute com: ./$(TARGET)"

# Compilar main.o
main.o: main.c paciente.h fila.h lista.h
	$(CC) $(CFLAGS) -c main.c

# Compilar paciente.o
paciente.o: paciente.c paciente.h pilha.h
	$(CC) $(CFLAGS) -c paciente.c

# Compilar fila.o
fila.o: fila.c fila.h paciente.h lista.h
	$(CC) $(CFLAGS) -c fila.c

# Compilar lista.o
lista.o: lista.c lista.h paciente.h
	$(CC) $(CFLAGS) -c lista.c

# Compilar pilha.o
pilha.o: pilha.c pilha.h
	$(CC) $(CFLAGS) -c pilha.c

# Limpar arquivos objeto e executável
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Arquivos de compilação removidos."

# Limpar tudo incluindo arquivos de dados
cleanall: clean
	rm -f Lista_de_pacientes.json Fila_da_triagem.json
	@echo "Todos os arquivos removidos (incluindo dados salvos)."

# Executar o programa
run: $(TARGET)
	./$(TARGET)

# Recompilar tudo
rebuild: clean all

# Verificar dependências
check:
	@echo "Verificando arquivos fonte..."
	@ls -1 *.c *.h 2>/dev/null || echo "Arquivos não encontrados"

# Ajuda
help:
	@echo "Makefile - Sistema de Gestão Hospitalar"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make          - Compila o projeto"
	@echo "  make all      - Compila o projeto (igual a make)"
	@echo "  make clean    - Remove arquivos objeto e executável"
	@echo "  make cleanall - Remove tudo incluindo dados salvos"
	@echo "  make run      - Compila e executa o programa"
	@echo "  make rebuild  - Recompila tudo do zero"
	@echo "  make check    - Verifica os arquivos fonte"
	@echo "  make help     - Mostra esta mensagem de ajuda"

# Declarar targets que não são arquivos
.PHONY: all clean cleanall run rebuild check help