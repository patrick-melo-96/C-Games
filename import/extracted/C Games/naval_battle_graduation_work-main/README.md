# Batalha Naval - Trabalho de Graduação

Sistema de Batalha Naval em C demonstrando arrays bidimensionais, structs, algoritmos de validação e padrões geométricos.

## 🚀 Início Rápido

```bash
make          # Compilar
./batalha_naval   # Executar
```

## 📋 Estrutura do Projeto

### Níveis Implementados

**Nível Novato** - Tabuleiro 10x10 com 2 navios (horizontal e vertical)  
**Nível Aventureiro** - 4 navios incluindo diagonais (\ e /)  
**Nível Mestre** - 3 habilidades especiais (Cone, Cruz, Octaedro)

### Arquivos Principais

- `batalha_naval.c` - Código-fonte principal
- `Makefile` - Automação de build

## 🎯 Conceitos Demonstrados

- Arrays bidimensionais (matrizes 10x10)
- Estruturas de dados (struct Coordenada)
- Funções e modularização
- Algoritmos de validação e posicionamento
- Padrões geométricos (distância de Manhattan)
- Sistema de coordenadas (base-1 para usuário, base-0 internamente)

## 📊 Algoritmos Principais

### Posicionamento de Navios
| Orientação (descrição) | Código no programa | Algoritmo |
|-------------------------|--------------------|-----------|
| Horizontal | `'H'` | `tabuleiro[lin][col+k] = NAVIO` |
| Vertical | `'V'` | `tabuleiro[lin+k][col] = NAVIO` |
| Diagonal descendente (\) | `'D'` | `tabuleiro[lin+k][col+k] = NAVIO` |
| Diagonal ascendente (/) | `'E'` | `tabuleiro[lin-k][col+k] = NAVIO` |

### Habilidades Especiais

**Cone** - Expansão triangular de cima para baixo
```
   5
  5 5 5
 5 5 5 5 5
5 5 5 5 5 5 5
```

**Cruz** - Linhas perpendiculares no centro
```
   5
   5
5 5 5 5 5
   5
   5
```

**Octaedro** - Losango (distância de Manhattan ≤ raio)
```
   5
  5 5 5
 5 5 5 5 5
  5 5 5
   5
```
Nota: No programa, habilidades (5) aparecem em vermelho e navios (3) em azul com ANSI.

## 🔧 Funções Principais

```c
void inicializarTabuleiro(int tabuleiro[10][10]);
void exibirTabuleiro(int tabuleiro[10][10]);
int validarPosicao(int tabuleiro[10][10], int linha, int coluna, int tamanho, char orientacao);
int posicionarNavio(int tabuleiro[10][10], Coordenada inicio, int tamanho, char orientacao);
void criarHabilidadeCone(int matriz[7][7], int tamanho);
void criarHabilidadeCruz(int matriz[7][7], int tamanho);
void criarHabilidadeOctaedro(int matriz[7][7], int tamanho);
void aplicarHabilidade(int tabuleiro[10][10], int habilidade[7][7], int tamanho, Coordenada origem);
void exibirMatrizHabilidade(int matriz[7][7], int tamanho);
```

## 📝 Exemplo de Uso

O programa demonstra automaticamente os 3 níveis:

1. **Fase 1** - Inicializa tabuleiro e posiciona 2 navios básicos
2. **Fase 2** - Adiciona 2 navios diagonais (total: 4 navios)
3. **Fase 3** - Demonstra cada habilidade especial individualmente (o padrão 7x7 é exibido antes; a área de efeito sobrepõe navios com o valor 5)

### Visualização do Tabuleiro
```
     1  2  3  4  5  6  7  8  9 10
 1:  0  0  0  0  0  0  0  0  0  0
 2:  0  0  0  0  0  0  0  0  0  0
 3:  0  3  3  3  0  0  0  0  0  0
 4:  0  0  0  0  0  0  0  0  0  0
 5:  0  0  0  0  0  0  0  0  0  0
 6:  0  0  0  0  0  0  0  3  0  0
 7:  0  0  0  0  0  0  0  3  0  0
 8:  3  0  0  0  0  0  0  3  0  0
 9:  0  3  0  0  0  0  0  0  0  0
10:  0  0  3  0  0  0  0  0  0  0

Legenda: 0 = Água | 3 = Navio | 5 = Habilidade
```

## 🧪 Validações Implementadas

- ✅ Limites do tabuleiro (coordenadas 1-10)
- ✅ Sobreposição de navios
- ✅ Tamanho válido para cada orientação
- ✅ Conversão base-1 (usuário) ↔ base-0 (array)
- ✅ Aplicação de habilidades respeitando limites

## 📈 Complexidade

| Operação | Complexidade | Justificativa |
|----------|--------------|---------------|
| Inicializar tabuleiro | O(n²) | n=10, loop duplo |
| Posicionar navio | O(k) | k=3, linear no tamanho |
| Criar habilidade | O(m²) | m=7, loop duplo |
| Exibir tabuleiro | O(n²) | n=10, loop duplo |

## 🛠️ Compilação

### Makefile (recomendado)
```bash
make          # Compilar
make run      # Compilar e executar
make clean    # Limpar arquivos compilados
make rebuild  # Recompilar do zero
```

### Manual
```bash
gcc -Wall -Wextra -std=c11 -o batalha_naval batalha_naval.c
./batalha_naval
```

## 📚 Estrutura do Código

```c
// Constantes
#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define AGUA 0
#define NAVIO 3
#define HABILIDADE 5

// Estruturas
typedef struct {
    int linha;
    int coluna;
} Coordenada;

// Coordenadas: base-1 (1-10) no display, base-0 (0-9) nos arrays
```

## ✅ Status

| Item | Status |
|------|--------|
| Nível Novato | ✅ Completo |
| Nível Aventureiro | ✅ Completo |
| Nível Mestre | ✅ Completo |
| Validações | ✅ Implementadas |
| Documentação | ✅ Completa |
| Compilação | ✅ Sem erros/warnings |

## 📅 Informações

**Autor:** Patrick Júlio Bezerra de Melo  
**Data:** 31 de Outubro de 2025  
**Linguagem:** C (C11)  
**Compilador:** GCC 13.3.0  
**Plataforma:** Linux (Ubuntu 24.04)

## 📄 Licença

Projeto acadêmico desenvolvido para fins educacionais.
