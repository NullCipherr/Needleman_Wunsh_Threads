# Compilador e flags
CC = gcc
CFLAGS = -Wall -pthread -Iinclude

# Diretórios
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Arquivos de código-fonte
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Arquivos objeto
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Executável
EXECUTABLE = $(BIN_DIR)/needleman_wunsch

# Regra padrão
all: $(EXECUTABLE)

# Regra para compilar o executável
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Compilação concluída com sucesso! O executável foi gerado em $(EXECUTABLE)."

# Regra para compilar arquivos .c em .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar arquivos gerados
clean:
	rm -rf $(BUILD_DIR)/*.o $(EXECUTABLE)
	@echo "Arquivos temporários e executável removidos com sucesso."

# Regra para limpar tudo
.PHONY: clean
