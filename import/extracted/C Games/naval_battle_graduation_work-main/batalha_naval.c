/*
 * BATALHA NAVAL - TRABALHO DE GRADUAÇÃO
 * Autor: Patrick Júlio Bezerra de Melo
 * Data: 31 de Outubro de 2025
 */

#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define AGUA 0
#define NAVIO 3
#define HABILIDADE 5

// Coordenadas usam base-1 (1-10) para o usuário, mas base-0 (0-9) internamente
typedef struct {
    int linha;
    int coluna;
} Coordenada;

// Protótipos
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void limparTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
int validarPosicao(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                   int linha, int coluna, int tamanho, char orientacao);
int posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                    Coordenada inicio, int tamanho, char orientacao);
void criarHabilidadeCone(int matriz[7][7], int tamanho);
void criarHabilidadeCruz(int matriz[7][7], int tamanho);
void criarHabilidadeOctaedro(int matriz[7][7], int tamanho);
void exibirMatrizHabilidade(int matriz[7][7], int tamanho);
// Exibe matriz de habilidade 7x7
void exibirMatrizHabilidade(int matriz[7][7], int tamanho) {
    for(int i = 0; i < tamanho; i++) {
        printf("   ");
        for(int j = 0; j < tamanho; j++) {
            printf("  %d ", matriz[i][j]);
        }
        printf("\n");
    }
}
void aplicarHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                       int habilidade[7][7], int tamanhoHabilidade, Coordenada origem);
void aguardarEnter();
void limparTela();
void jogarBatalhaNaval();

void aguardarEnter() {
    printf("\nPressione ENTER para continuar...");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limparTela() {
#ifdef _WIN32
    if (system("cls") == -1) {
        fputs("\n\n", stdout);
    }
#else
    if (system("clear 2>/dev/null || printf '\\n\\n'") == -1) {
        fputs("\n\n", stdout);
    }
#endif
}

int main() {
    printf("\n");
    printf("BATALHA NAVAL - TRABALHO DE GRADUAÇÃO\n");
    printf("\n");
    printf("Batalha Naval com três níveis de complexidade crescente:\n");
    printf("• Novato: Navios alinhados horizontalmente e verticalmente\n");
    printf("• Aventureiro: Navios diagonais adicionam nova dimensão tática ao oceano\n");
    printf("• Mestre: Habilidades especiais com áreas de efeito transformam cada ataque em decisão estratégica\n\n");
    
    aguardarEnter();
    jogarBatalhaNaval();
    
    printf("\n\n");
    printf("OBRIGADO POR JOGAR!\n");
    printf("\n");
    
    return 0;
}

void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for(int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for(int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("\n    ");
    for(int col = 1; col <= TAMANHO_TABULEIRO; col++) {
        printf("  %d ", col);
    }
    printf("\n");
    for(int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d: ", i+1);
        for(int j = 0; j < TAMANHO_TABULEIRO; j++) {
            // Se for habilidade (5), imprime em vermelho
            if(tabuleiro[i][j] == 5) {
                printf("  \033[1;31m%d\033[0m ", tabuleiro[i][j]); // vermelho ANSI
            }
            // Se for navio (3), imprime em azul
            else if(tabuleiro[i][j] == 3) {
                printf("  \033[1;34m%d\033[0m ", tabuleiro[i][j]); // azul ANSI
            }
            // Água ou outros valores, imprime normal
            else {
                printf("  %d ", tabuleiro[i][j]);
            }
        }
        printf("\n");
    }
    // Legenda também colorida para facilitar identificação visual
    printf("Legenda: 0 = Água | \033[1;34m3\033[0m = Navio | \033[1;31m5\033[0m = Habilidade\n");
}

void limparTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for(int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for(int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if(tabuleiro[i][j] == HABILIDADE) {
                tabuleiro[i][j] = AGUA;
            }
        }
    }
}

// Recebe coordenadas base-1, converte para base-0
int validarPosicao(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                   int linha, int coluna, int tamanho, char orientacao) {
    int lin = linha - 1;
    int col = coluna - 1;
    
    if(orientacao == 'H') {
        if(col + tamanho > TAMANHO_TABULEIRO) return 0;
        for(int j = col; j < col + tamanho; j++) {
            if(tabuleiro[lin][j] != AGUA) return 0;
        }
    }
    else if(orientacao == 'V') {
        if(lin + tamanho > TAMANHO_TABULEIRO) return 0;
        for(int i = lin; i < lin + tamanho; i++) {
            if(tabuleiro[i][col] != AGUA) return 0;
        }
    }
    else if(orientacao == 'D') {
        if(lin + tamanho > TAMANHO_TABULEIRO || col + tamanho > TAMANHO_TABULEIRO) return 0;
        for(int k = 0; k < tamanho; k++) {
            if(tabuleiro[lin + k][col + k] != AGUA) return 0;
        }
    }
    else if(orientacao == 'E') {
        if(lin - tamanho + 1 < 0 || col + tamanho > TAMANHO_TABULEIRO) return 0;
        for(int k = 0; k < tamanho; k++) {
            if(tabuleiro[lin - k][col + k] != AGUA) return 0;
        }
    }
    
    return 1;
}

// Recebe coordenadas base-1, converte para base-0
int posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                    Coordenada inicio, int tamanho, char orientacao) {
    if(!validarPosicao(tabuleiro, inicio.linha, inicio.coluna, tamanho, orientacao)) {
        return 0;
    }
    
    int lin = inicio.linha - 1;
    int col = inicio.coluna - 1;
    
    if(orientacao == 'H') {
        for(int j = col; j < col + tamanho; j++) {
            tabuleiro[lin][j] = NAVIO;
        }
    } else if(orientacao == 'V') {
        for(int i = lin; i < lin + tamanho; i++) {
            tabuleiro[i][col] = NAVIO;
        }
    } else if(orientacao == 'D') {
        for(int k = 0; k < tamanho; k++) {
            tabuleiro[lin + k][col + k] = NAVIO;
        }
    } else if(orientacao == 'E') {
        for(int k = 0; k < tamanho; k++) {
            tabuleiro[lin - k][col + k] = NAVIO;
        }
    }
    
    return 1;
}

void criarHabilidadeCone(int matriz[7][7], int tamanho) {
    int centro = tamanho / 2;
    for(int i = 0; i < tamanho; i++) {
        for(int j = 0; j < tamanho; j++) {
            if(i <= centro) {
                matriz[i][j] = (j >= centro - i && j <= centro + i) ? 5 : 0;
            } else {
                matriz[i][j] = 5;
            }
        }
    }
}

void criarHabilidadeCruz(int matriz[7][7], int tamanho) {
    int centro = tamanho / 2;
    for(int i = 0; i < tamanho; i++) {
        for(int j = 0; j < tamanho; j++) {
            matriz[i][j] = (i == centro || j == centro) ? 5 : 0;
        }
    }
}

void criarHabilidadeOctaedro(int matriz[7][7], int tamanho) {
    int centro = tamanho / 2;
    for(int i = 0; i < tamanho; i++) {
        for(int j = 0; j < tamanho; j++) {
            int distancia = abs(i - centro) + abs(j - centro);
            matriz[i][j] = (distancia <= centro) ? 5 : 0;
        }
    }
}

// Recebe coordenadas base-1, converte para base-0
void aplicarHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                       int habilidade[7][7], int tamanhoHabilidade, Coordenada origem) {
    int lin = origem.linha - 1;
    int col = origem.coluna - 1;
    int offset = tamanhoHabilidade / 2;
    
    for(int i = 0; i < tamanhoHabilidade; i++) {
        for(int j = 0; j < tamanhoHabilidade; j++) {
            int linhaTab = lin - offset + i;
            int colunaTab = col - offset + j;
            
            if(linhaTab >= 0 && linhaTab < TAMANHO_TABULEIRO && 
               colunaTab >= 0 && colunaTab < TAMANHO_TABULEIRO) {
                if(habilidade[i][j] == 5) {
                    tabuleiro[linhaTab][colunaTab] = HABILIDADE;
                }
            }
        }
    }
}

void jogarBatalhaNaval() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    int habilidade[7][7];
    
    // FASE 1: NÍVEL NOVATO
    printf("\n");
    printf("FASE 1: NÍVEL NOVATO\n");
    printf("Conceitos Básicos de Posicionamento\n");
    printf("\n");
    printf("Estruturas: Array 2D (matriz 10x10), struct Coordenada\n");
    printf("Operações: Loops aninhados, validação de limites\n");
    printf("Navios: Horizontal e Vertical\n\n");
    
    aguardarEnter();
    limparTela();
    
    printf("\n[PASSO 1] Inicializando tabuleiro 10x10 com água...\n");
    inicializarTabuleiro(tabuleiro);
    printf("✓ Tabuleiro inicializado!\n");
    
    printf("\n[PASSO 2] Posicionando navios básicos (horizontal e vertical)...\n");
    
    Coordenada navio1 = {3, 2};
    if(posicionarNavio(tabuleiro, navio1, TAMANHO_NAVIO, 'H')) {
        printf("✓ Navio 1 (Horizontal) posicionado em (%d, %d) - tamanho %d\n", 
               navio1.linha, navio1.coluna, TAMANHO_NAVIO);
    }
    
    Coordenada navio2 = {6, 8};
    if(posicionarNavio(tabuleiro, navio2, TAMANHO_NAVIO, 'V')) {
        printf("✓ Navio 2 (Vertical) posicionado em (%d, %d) - tamanho %d\n", 
               navio2.linha, navio2.coluna, TAMANHO_NAVIO);
    }
    
    printf("\n[RESULTADO] Estado atual do tabuleiro:\n");
    exibirTabuleiro(tabuleiro);
    
    printf("\n[VALIDAÇÃO FASE 1] ✓ Nível Novato concluído!\n");
    printf("• Arrays 2D: Tabuleiro 10x10 inicializado\n");
    printf("• Função posicionarNavio(): 2 navios (Horizontal + Vertical) inseridos\n");
    printf("• Função validarPosicao(): Limites e sobreposição verificados\n");
    
    aguardarEnter();
    limparTela();
    
    // FASE 2: NÍVEL AVENTUREIRO
    printf("\n");
    printf("FASE 2: NÍVEL AVENTUREIRO\n");
    printf("Adicionando Navios Diagonais\n");
    printf("\n");
    printf("Novos modos: Diagonal descendente '\\' e Diagonal ascendente '/'\n");
    printf("Controle: Condicionais if/else para 4 orientações\n");
    printf("Validação: Cálculo de índices incrementais (i+k, j+k)\n\n");
    
    aguardarEnter();
    limparTela();
    
    printf("\n[PASSO 3] Adicionando navios diagonais ao tabuleiro...\n");
    
    Coordenada navio3 = {2, 6};
    if(posicionarNavio(tabuleiro, navio3, TAMANHO_NAVIO, 'D')) {
        printf("✓ Navio 3 (Diagonal \\) posicionado em (%d, %d) - tamanho %d\n", 
               navio3.linha, navio3.coluna, TAMANHO_NAVIO);
    }
    
    Coordenada navio4 = {8, 1};
    if(posicionarNavio(tabuleiro, navio4, TAMANHO_NAVIO, 'E')) {
        printf("✓ Navio 4 (Diagonal /) posicionado em (%d, %d) - tamanho %d\n", 
               navio4.linha, navio4.coluna, TAMANHO_NAVIO);
    }
    
    printf("\n[RESULTADO] Tabuleiro completo com todos os navios:\n");
    exibirTabuleiro(tabuleiro);
    
    printf("\n[VALIDAÇÃO FASE 2] ✓ Nível Aventureiro concluído!\n");
    printf("• Total: 4 navios (tamanho 3) - Horizontal, Vertical, Diagonal descendente, Diagonal ascendente\n");
    printf("• Função validarPosicao(): 4 casos tratados\n");
    printf("• Algoritmo: Incremento condicional de índices\n");
    
    aguardarEnter();
    limparTela();
    
    // FASE 3: NÍVEL MESTRE
    printf("\n");
    printf("FASE 3: NÍVEL MESTRE\n");
    printf("Habilidades Especiais com Áreas de Efeito\n");
    printf("\n");
    printf("Matrizes 7x7: Padrões geométricos programáticos\n");
    printf("Algoritmos: Cone (expansão), Cruz (eixos), Octaedro (Manhattan)\n");
        printf("Aplicação: Centraliza matriz 7x7 em coordenada-alvo do tabuleiro 10x10\n");
        printf("           usando offset de 3 posições (centro da matriz)\n\n");
    
    aguardarEnter();
    limparTela();
    
    Coordenada origem;
    
    // HABILIDADE 1: CONE
    printf("\n");
    printf("HABILIDADE 1: CONE\n");
    printf("\n");
    printf("Formato: Cone invertido (topo→base)\n");
    printf("Lógica: if(i<=centro) largura=2*i+1; else largura=max\n\n");
    
    limparTabuleiro(tabuleiro);
    criarHabilidadeCone(habilidade, 7);
    printf("[PADRÃO 7x7] Habilidade CONE:\n");
    exibirMatrizHabilidade(habilidade, 7);
    origem.linha = 5;
    origem.coluna = 5;
    aplicarHabilidade(tabuleiro, habilidade, 7, origem);
    printf("[RESULTADO] Tabuleiro com habilidade CONE aplicada:\n");
    exibirTabuleiro(tabuleiro);
    
    aguardarEnter();
    limparTela();
    
    // HABILIDADE 2: CRUZ
    printf("\n");
    printf("HABILIDADE 2: CRUZ\n");
    printf("\n");
    printf("Formato: Cruz (eixos perpendiculares)\n");
    printf("Lógica: if(i==centro || j==centro) valor=1\n\n");
    
    limparTabuleiro(tabuleiro);
    criarHabilidadeCruz(habilidade, 7);
    printf("[PADRÃO 7x7] Habilidade CRUZ:\n");
    exibirMatrizHabilidade(habilidade, 7);
    origem.linha = 4;
    origem.coluna = 4;
    aplicarHabilidade(tabuleiro, habilidade, 7, origem);
    printf("[RESULTADO] Tabuleiro com habilidade CRUZ aplicada:\n");
    exibirTabuleiro(tabuleiro);
    
    aguardarEnter();
    limparTela();
    
    // HABILIDADE 3: OCTAEDRO
    printf("\n");
    printf("HABILIDADE 3: OCTAEDRO\n");
    printf("\n");
    printf("Formato: Losango (vista frontal)\n");
    printf("Lógica: distancia=|i-centro|+|j-centro|; if(d<=raio) valor=1\n\n");
    
    limparTabuleiro(tabuleiro);
    criarHabilidadeOctaedro(habilidade, 7);
    printf("[PADRÃO 7x7] Habilidade OCTAEDRO:\n");
    exibirMatrizHabilidade(habilidade, 7);
    origem.linha = 6;
    origem.coluna = 6;
    aplicarHabilidade(tabuleiro, habilidade, 7, origem);
    printf("[RESULTADO] Tabuleiro com habilidade OCTAEDRO aplicada:\n");
    exibirTabuleiro(tabuleiro);
    
    printf("\n[VALIDAÇÃO FASE 3] ✓ Nível Mestre concluído!\n");
    printf("• 3 funções: criarHabilidade[Cone|Cruz|Octaedro]()\n");
    printf("• Arrays 2D: Padrões gerados com loops+condicionais\n");
    printf("• Função aplicarHabilidade(): Offset e validação de limites\n");
    
    aguardarEnter();
    limparTela();
    
    // CONCLUSÃO
    printf("\n");
    printf("PARABÉNS! JOGO CONCLUÍDO!\n");
    printf("\n");
    printf("RESUMO DO SISTEMA:\n\n");
    printf("Estruturas de Dados:\n");
    printf("• int[10][10]: Tabuleiro principal\n");
    printf("• struct Coordenada: {linha, coluna}\n");
    printf("• int[7][7]: Matrizes de habilidades\n\n");
    printf("Funções Implementadas:\n");
    printf("• inicializarTabuleiro(): Loop 2D para zerar matriz\n");
    printf("• validarPosicao(): 4 casos (Horizontal/Vertical/Diagonal descendente/Diagonal ascendente) com condicionais\n");
    printf("• posicionarNavio(): Inserção controlada por orientação\n");
    printf("• criar[Cone|Cruz|Octaedro](): Padrões geométricos\n");
    printf("• aplicarHabilidade(): Mapeamento matriz→tabuleiro\n\n");
    printf("Conceitos Aplicados:\n");
    printf("• Arrays 2D, Structs, Loops aninhados, Condicionais\n");
    printf("• Validação de limites, Algoritmos geométricos\n\n");
    
    aguardarEnter();
}
