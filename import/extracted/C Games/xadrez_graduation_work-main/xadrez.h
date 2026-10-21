#ifndef XADREZ_H
#define XADREZ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAMANHO_TABULEIRO 8

// Tipos de peças
typedef enum {
    VAZIO,
    TORRE,
    BISPO,
    RAINHA,
    CAVALO
} TipoPeca;

// Estrutura para representar uma posição no tabuleiro
typedef struct {
    int linha;
    int coluna;
} Posicao;

// Estrutura para representar uma peça
typedef struct {
    TipoPeca tipo;
    Posicao posicao;
} Peca;

// Funções do tabuleiro
void inicializar_tabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void imprimir_tabuleiro(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void marcar_movimentos(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], Posicao origem);

// Funções de validação de movimentos
bool movimento_valido_torre(Posicao origem, Posicao destino);
bool movimento_valido_bispo(Posicao origem, Posicao destino);
bool movimento_valido_rainha(Posicao origem, Posicao destino);

// Funções auxiliares
bool posicao_valida(Posicao pos);
char obter_simbolo_peca(TipoPeca tipo);
const char* obter_nome_peca(TipoPeca tipo);

#endif
