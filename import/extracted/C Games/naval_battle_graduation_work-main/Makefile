# ============================================================================
# Makefile para o Jogo de Batalha Naval
# ============================================================================

# Compilador e flags
CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -g
TARGET := batalha_naval

# Arquivos fonte e objetos (facilita expansão futura)
SRCS := batalha_naval.c
OBJS := $(SRCS:.c=.o)
RM   := rm -f

# Regra padrão: compilar o programa
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJS)
	@echo "Compilando $(TARGET)..."
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Compilação concluída com sucesso!"

# Regra de compilação para objetos
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Regra para executar o programa
run: $(TARGET)
	@echo "Executando $(TARGET)..."
	./$(TARGET)

# Regra para limpar arquivos compilados
clean:
	@echo "Limpando arquivos..."
	$(RM) $(TARGET) $(OBJS)
	@echo "Limpeza concluída!"

# Regra para recompilar (limpar e compilar novamente)
rebuild: clean all

# Limpeza completa (inclui backups)
distclean: clean
	$(RM) *.backup *.bak

# Marca as regras que não geram arquivos
.PHONY: all run clean rebuild distclean
