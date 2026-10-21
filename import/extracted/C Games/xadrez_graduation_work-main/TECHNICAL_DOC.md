# Documentação Técnica - Programa de Xadrez

## Visão Geral

Este documento descreve a implementação do programa de xadrez que simula movimentos de peças usando técnicas avançadas de programação em C.

## Requisitos Implementados

### 1. Recursividade (Torre, Bispo e Rainha)

#### Torre (Rook)
- **Função**: `movimentoTorreRecursivo()`
- **Implementação**: Função recursiva que imprime a direção do movimento e chama a si mesma com `casas - 1`
- **Caso Base**: Quando `casas <= 0`, a recursão termina
- **Movimento**: 5 casas em linha reta (direita)

#### Bispo (Bishop) - Recursivo
- **Função**: `movimentoBispoRecursivo()`
- **Implementação**: Função recursiva que imprime direções vertical e horizontal (movimento diagonal)
- **Caso Base**: Quando `casas <= 0`, a recursão termina
- **Movimento**: 4 casas na diagonal (cima e direita)

#### Rainha (Queen)
- **Função**: `movimentoRainhaRecursivo()`
- **Implementação**: Função recursiva similar ao bispo, combinando movimentos
- **Caso Base**: Quando `casas <= 0`, a recursão termina
- **Movimento**: 6 casas na diagonal (cima e direita)

### 2. Loops Complexos (Cavalo)

#### Cavalo (Knight)
- **Função**: `movimentoCavaloComplexo()`
- **Implementação**: 
  - Loop externo controla movimento vertical (2 casas para cima)
  - Loop interno controla movimento horizontal (1 casa para direita)
  - Usa múltiplas variáveis de controle: `i`, `j`, `contadorVertical`, `contadorHorizontal`
  - Implementa `continue` para pular iterações enquanto não completa movimento vertical
  - Implementa `break` para sair dos loops quando completa os movimentos
- **Movimento**: Formato "L" característico (2 para cima, 1 para direita)

### 3. Loops Aninhados (Bispo)

#### Bispo (Bishop) - Loops Aninhados
- **Função**: `movimentoBispoLoops()`
- **Implementação**:
  - Loop externo (`for i`): controla movimento vertical
  - Loop interno (`for j`): controla movimento horizontal
  - Loop externo executa `casas` iterações (4 vezes)
  - Loop interno executa 1 iteração por cada iteração do externo
- **Movimento**: 4 casas na diagonal (cima e direita)

## Estrutura de Dados

### Constantes Definidas
```c
#define CASAS_TORRE 5              // Número de casas para Torre
#define CASAS_BISPO 4              // Número de casas para Bispo
#define CASAS_RAINHA 6             // Número de casas para Rainha
#define CASAS_CAVALO_VERTICAL 2    // Casas verticais do Cavalo
#define CASAS_CAVALO_HORIZONTAL 1  // Casas horizontais do Cavalo
```

### Variáveis Utilizadas
- **Tipo int**: Para contadores, índices de loops e número de casas
- **Tipo const char***: Para strings de direção ("Cima", "Baixo", "Esquerda", "Direita")

## Formato de Saída

O programa imprime as direções de movimento usando o formato especificado:
- `printf("Cima\n");`
- `printf("Baixo\n");`
- `printf("Esquerda\n");`
- `printf("Direita\n");`

Cada peça tem sua saída separada por uma linha em branco (`printf("\n");`).

## Requisitos Não-Funcionais

### Performance
- Recursão limitada a um número pequeno de casas (máximo 6 para Rainha)
- Evita stack overflow mantendo profundidade de recursão controlada
- Loops têm complexidade O(n) onde n é o número de casas

### Documentação
- Comentários detalhados em todas as funções
- Explicação da lógica recursiva
- Descrição do funcionamento dos loops complexos
- Documentação de todas as variáveis e condições

### Legibilidade
- Código bem organizado e indentado
- Nomes de variáveis descritivos
- Estrutura clara e fácil de entender
- Separação lógica entre diferentes peças

## Como Compilar

### Usando Makefile
```bash
make            # Compila o programa
make run        # Compila e executa
make clean      # Remove executável
make rebuild    # Limpa e recompila
```

### Compilação Manual
```bash
gcc -Wall -Wextra -std=c99 -o xadrez xadrez.c
```

## Como Executar

```bash
./xadrez
```

## Exemplo de Saída

```
Torre:
Direita
Direita
Direita
Direita
Direita

Bispo (Recursivo):
Cima
Direita
Cima
Direita
Cima
Direita
Cima
Direita

Bispo (Loops Aninhados):
Cima
Direita
Cima
Direita
Cima
Direita
Cima
Direita

Rainha:
Cima
Direita
Cima
Direita
Cima
Direita
Cima
Direita
Cima
Direita
Cima
Direita

Cavalo:
Cima
Cima
Direita
```

## Arquivos do Projeto

- **xadrez.c**: Código fonte principal
- **Makefile**: Script de compilação
- **.gitignore**: Configuração Git
- **README.md**: Documentação do usuário
- **TECHNICAL_DOC.md**: Esta documentação técnica

## Requisitos do Sistema

- Compilador GCC com suporte a C99 ou superior
- Sistema operacional Unix/Linux/macOS ou Windows com MinGW

## Notas de Implementação

1. Todas as funções seguem o princípio de responsabilidade única
2. Não há validação de entrada pois os valores são constantes definidas no código
3. O programa não implementa regras completas de xadrez, apenas movimentos básicos
4. Foco em demonstração de técnicas de programação (recursão e loops complexos)
