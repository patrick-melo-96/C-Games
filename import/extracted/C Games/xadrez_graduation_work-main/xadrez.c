#include "xadrez.h"
#include <stdlib.h>

// Inicializa o tabuleiro vazio com '.'
void inicializar_tabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = '.';
        }
    }
}

// Imprime o tabuleiro formatado no terminal
void imprimir_tabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("\n  ");
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%c ", 'a' + i);
    }
    printf("\n");
    
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    
    printf("  ");
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%c ", 'a' + i);
    }
    printf("\n\n");
}

// Verifica se uma posição está dentro dos limites do tabuleiro
bool posicao_valida(Posicao pos) {
    return pos.linha >= 0 && pos.linha < TAMANHO_TABULEIRO &&
           pos.coluna >= 0 && pos.coluna < TAMANHO_TABULEIRO;
}

// Retorna o caractere representativo da peça
char obter_simbolo_peca(TipoPeca tipo) {
    switch (tipo) {
        case TORRE:  return 'T';
        case BISPO:  return 'B';
        case RAINHA: return 'R';
        case CAVALO: return 'C';
        default:     return '.';
    }
}

// Retorna o nome textual da peça
const char* obter_nome_peca(TipoPeca tipo) {
    switch (tipo) {
        case TORRE:  return "Torre";
        case BISPO:  return "Bispo";
        case RAINHA: return "Rainha";
        case CAVALO: return "Cavalo";
        default:     return "Vazio";
    }
}

// Valida movimento da Torre (linha reta horizontal ou vertical)
bool movimento_valido_torre(Posicao origem, Posicao destino) {
    if (!posicao_valida(origem) || !posicao_valida(destino)) return false;
    return (origem.linha == destino.linha && origem.coluna != destino.coluna) ||
           (origem.coluna == destino.coluna && origem.linha != destino.linha);
}

// Valida movimento do Bispo (diagonal)
bool movimento_valido_bispo(Posicao origem, Posicao destino) {
    if (!posicao_valida(origem) || !posicao_valida(destino)) return false;
    int diff_linha = abs(destino.linha - origem.linha);
    int diff_coluna = abs(destino.coluna - origem.coluna);
    return diff_linha == diff_coluna && diff_linha > 0;
}

// Valida movimento da Rainha (combina Torre e Bispo)
bool movimento_valido_rainha(Posicao origem, Posicao destino) {
    if (!posicao_valida(origem) || !posicao_valida(destino)) return false;
    return movimento_valido_torre(origem, destino) || movimento_valido_bispo(origem, destino);
}

// Valida movimento do Cavalo (formato em "L": 2 casas em uma direção e 1 na perpendicular)
bool movimento_valido_cavalo(Posicao origem, Posicao destino) {
    if (!posicao_valida(origem) || !posicao_valida(destino)) return false;
    int diff_linha = abs(destino.linha - origem.linha);
    int diff_coluna = abs(destino.coluna - origem.coluna);
    return (diff_linha == 2 && diff_coluna == 1) || (diff_linha == 1 && diff_coluna == 2);
}

// Marca os movimentos possíveis da peça escolhida no tabuleiro
void marcar_movimentos(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], Posicao origem) {
    TipoPeca tipo;
    char simbolo = tabuleiro[origem.linha][origem.coluna];
    
    switch (simbolo) {
        case 'T': tipo = TORRE; break;
        case 'B': tipo = BISPO; break;
        case 'R': tipo = RAINHA; break;
        case 'C': tipo = CAVALO; break;
        default:
            printf("Nenhuma peça válida nesta posição!\n");
            return;
    }
    
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            Posicao destino = {i, j};
            bool valido = false;
            
            switch (tipo) {
                case TORRE:  valido = movimento_valido_torre(origem, destino); break;
                case BISPO:  valido = movimento_valido_bispo(origem, destino); break;
                case RAINHA: valido = movimento_valido_rainha(origem, destino); break;
                case CAVALO: valido = movimento_valido_cavalo(origem, destino); break;
                case VAZIO: break;
            }
            
            if (valido && tabuleiro[i][j] == '.') {
                tabuleiro[i][j] = '*';
            }
        }
    }
}

int main() {
    char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    int opcao;
    int c;
    
    printf("===========================================\n");
    printf("  JOGO DE XADREZ - MOVIMENTAÇÃO DE PEÇAS\n");
    printf("===========================================\n\n");
    
    do {
        printf("Escolha uma peça para demonstrar os movimentos:\n");
        printf("1 - Torre\n");
        printf("2 - Bispo\n");
        printf("3 - Rainha\n");
        printf("4 - Cavalo\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada inválida! Tente novamente.\n\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        while ((c = getchar()) != '\n' && c != EOF);
        
        if (opcao == 0) {
            printf("\nEncerrando o programa...\n");
            break;
        }
        
        if (opcao < 1 || opcao > 4) {
            printf("\nOpção inválida! Escolha entre 0 e 4.\n\n");
            continue;
        }
        
        TipoPeca tipo;
        switch (opcao) {
            case 1: tipo = TORRE; break;
            case 2: tipo = BISPO; break;
            case 3: tipo = RAINHA; break;
            case 4: tipo = CAVALO; break;
            default: continue;
        }
        
        char coluna_char;
        int linha_num;
        
        printf("\nDigite a posição da peça (ex: e4): ");
        if (scanf(" %c%d", &coluna_char, &linha_num) != 2) {
            printf("\nEntrada inválida! Tente novamente.\n\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        while ((c = getchar()) != '\n' && c != EOF);
        
        Posicao pos;
        pos.coluna = coluna_char - 'a';
        pos.linha = 8 - linha_num;
        
        if (!posicao_valida(pos)) {
            printf("\nPosição inválida! Use colunas de 'a' a 'h' e linhas de 1 a 8.\n\n");
            continue;
        }
        
        inicializar_tabuleiro(tabuleiro);
        tabuleiro[pos.linha][pos.coluna] = obter_simbolo_peca(tipo);
        
        printf("\n%s posicionada em %c%d:\n", obter_nome_peca(tipo), coluna_char, linha_num);
        imprimir_tabuleiro(tabuleiro);
        
        marcar_movimentos(tabuleiro, pos);
        
        printf("Movimentos possíveis (marcados com *):\n");
        imprimir_tabuleiro(tabuleiro);
        
        printf("Legenda:\n");
        printf("  %c = %s\n", obter_simbolo_peca(tipo), obter_nome_peca(tipo));
        printf("  * = Movimento possível\n");
        printf("  . = Casa vazia\n\n");
        
    } while (1);
    
    return 0;
}