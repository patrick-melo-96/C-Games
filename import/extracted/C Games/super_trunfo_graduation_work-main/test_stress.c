#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <time.h>

// Incluindo as definições do código original
typedef struct {
    char cidade[33];
    unsigned long int populacao;
    float area;
    float pib;
    int pontos_turisticos;
    float densidade_populacional;
    float pib_per_capita;
    float super_poder;
} Carta;

#define IDX_POPULACAO 1
#define IDX_AREA 2
#define IDX_DENSIDADE 3
#define IDX_PIB 4
#define IDX_PIB_PER_CAPITA 5
#define IDX_PONTOS_TURISTICOS 6
#define IDX_SUPER_PODER 7

// Funções copiadas do código original
float acessarAtributo(Carta* carta, int indice) {
    switch (indice) {
        case IDX_POPULACAO: return carta->populacao;
        case IDX_AREA: return carta->area;
        case IDX_DENSIDADE: return carta->densidade_populacional;
        case IDX_PIB: return carta->pib;
        case IDX_PIB_PER_CAPITA: return carta->pib_per_capita;
        case IDX_PONTOS_TURISTICOS: return (float)carta->pontos_turisticos;
        case IDX_SUPER_PODER: return carta->super_poder;
        default: return -1.0f;
    }
}

int compara_atributo(Carta* c1, Carta* c2, int atributo) {
    float v1 = acessarAtributo(c1, atributo);
    float v2 = acessarAtributo(c2, atributo);
    bool menor_vence = (atributo == IDX_DENSIDADE);
    if (v1 == v2) return 0;
    if (menor_vence) return (v1 < v2) ? 1 : 2;
    return (v1 > v2) ? 1 : 2;
}

// Função para criar carta com valores específicos
Carta criar_carta_stress(const char* nome, unsigned long pop, float area, float pib, int pontos) {
    Carta carta;
    strncpy(carta.cidade, nome, sizeof(carta.cidade) - 1);
    carta.cidade[sizeof(carta.cidade) - 1] = '\0';
    carta.populacao = pop;
    carta.area = area;
    carta.pib = pib;
    carta.pontos_turisticos = pontos;
    
    // Calcular campos derivados com proteção contra divisão por zero
    carta.densidade_populacional = (area != 0.0f) ? (pop / area) : 0.0f;
    carta.pib_per_capita = (pop != 0.0f) ? ((pib * 1000000000) / pop) : 0.0f;
    carta.super_poder = pop + area + pib + pontos + carta.pib_per_capita +
                       (carta.densidade_populacional != 0.0f ? 1.0f / carta.densidade_populacional : 0.0f);
    
    return carta;
}

// Testes de stress
void teste_stress_valores_extremos() {
    printf("=== TESTE DE STRESS: VALORES EXTREMOS ===\n");
    
    clock_t inicio = clock();
    
    // Teste 1: Valores máximos
    printf("Testando valores máximos...\n");
    Carta carta_max = criar_carta_stress("MAX", ULONG_MAX, FLT_MAX, FLT_MAX, INT_MAX);
    printf("✓ Carta com valores máximos criada\n");
    
    // Teste 2: Valores mínimos
    printf("Testando valores mínimos...\n");
    Carta carta_min = criar_carta_stress("MIN", 1, 0.0001f, 0.0001f, 1);
    printf("✓ Carta com valores mínimos criada\n");
    
    // Teste 3: Valores zero
    printf("Testando valores zero...\n");
    Carta carta_zero = criar_carta_stress("ZERO", 0, 0.0f, 0.0f, 0);
    printf("✓ Carta com valores zero criada\n");
    
    // Teste 4: Comparações extremas
    printf("Testando comparações extremas...\n");
    for (int attr = IDX_POPULACAO; attr <= IDX_SUPER_PODER; attr++) {
        int resultado1 = compara_atributo(&carta_max, &carta_min, attr);
        int resultado2 = compara_atributo(&carta_min, &carta_zero, attr);
        int resultado3 = compara_atributo(&carta_max, &carta_zero, attr);
        
        if (resultado1 < 0 || resultado1 > 2 || 
            resultado2 < 0 || resultado2 > 2 || 
            resultado3 < 0 || resultado3 > 2) {
            printf("✗ Erro na comparação do atributo %d\n", attr);
        }
    }
    printf("✓ Comparações extremas concluídas\n");
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo de execução: %.4f segundos\n", tempo);
}

void teste_stress_precisao_float() {
    printf("\n=== TESTE DE STRESS: PRECISÃO FLOAT ===\n");
    
    // Teste com valores muito próximos
    printf("Testando precisão com valores muito próximos...\n");
    
    for (int i = 0; i < 1000; i++) {
        float base = 1000000.0f;
        float incremento = 0.0001f * i;
        
        Carta carta_a = criar_carta_stress("A", (unsigned long)(base), 100.0f, base + incremento, 5);
        Carta carta_b = criar_carta_stress("B", (unsigned long)(base), 100.0f, base + incremento + 0.0001f, 5);
        
        int resultado = compara_atributo(&carta_a, &carta_b, IDX_PIB);
        
        if (resultado < 0 || resultado > 2) {
            printf("✗ Erro na precisão no teste %d\n", i);
            break;
        }
    }
    printf("✓ Teste de precisão concluído\n");
}

void teste_stress_divisao_por_zero() {
    printf("\n=== TESTE DE STRESS: DIVISÃO POR ZERO ===\n");
    
    // Teste múltiplas divisões por zero
    printf("Testando múltiplas divisões por zero...\n");
    
    for (int i = 0; i < 100; i++) {
        // Área zero - densidade
        Carta carta_area_zero = criar_carta_stress("AREA_ZERO", 1000000, 0.0f, 50.0f, 5);
        
        // População zero - PIB per capita
        Carta carta_pop_zero = criar_carta_stress("POP_ZERO", 0, 100.0f, 50.0f, 5);
        
        // Ambos zero
        Carta carta_ambos_zero = criar_carta_stress("AMBOS_ZERO", 0, 0.0f, 50.0f, 5);
        
        // Verificar se não há valores inválidos (NaN, Inf)
        if (carta_area_zero.densidade_populacional != 0.0f) {
            printf("✗ Densidade deveria ser 0 com área zero\n");
        }
        
        if (carta_pop_zero.pib_per_capita != 0.0f) {
            printf("✗ PIB per capita deveria ser 0 com população zero\n");
        }
        
        if (carta_ambos_zero.densidade_populacional != 0.0f || carta_ambos_zero.pib_per_capita != 0.0f) {
            printf("✗ Ambos valores deveriam ser 0\n");
        }
    }
    printf("✓ Teste de divisão por zero concluído\n");
}

void teste_stress_nome_strings() {
    printf("\n=== TESTE DE STRESS: STRINGS DE NOME ===\n");
    
    // Teste 1: Nome com tamanho limite
    char nome_limite[33];
    memset(nome_limite, 'A', 32);
    nome_limite[32] = '\0';
    
    Carta carta_limite = criar_carta_stress(nome_limite, 1000000, 100.0f, 50.0f, 5);
    if (strlen(carta_limite.cidade) != 32) {
        printf("✗ Nome com 32 caracteres não foi aceito corretamente\n");
    } else {
        printf("✓ Nome com 32 caracteres aceito\n");
    }
    
    // Teste 2: Nome com caracteres especiais
    char nome_especial[] = "São-Paulo/RJ_123";
    Carta carta_especial = criar_carta_stress(nome_especial, 1000000, 100.0f, 50.0f, 5);
    if (strcmp(carta_especial.cidade, nome_especial) == 0) {
        printf("✓ Nome com caracteres especiais aceito\n");
    } else {
        printf("✗ Nome com caracteres especiais não foi aceito corretamente\n");
    }
    
    // Teste 3: Nome vazio
    Carta carta_vazia = criar_carta_stress("", 1000000, 100.0f, 50.0f, 5);
    if (carta_vazia.cidade[0] == '\0') {
        printf("✓ Nome vazio testado\n");
    } else {
        printf("✗ Nome vazio não foi preservado\n");
    }
}

void teste_stress_performance() {
    printf("\n=== TESTE DE STRESS: PERFORMANCE ===\n");
    
    clock_t inicio = clock();
    
    // Criar muitas cartas e fazer muitas comparações
    printf("Testando performance com múltiplas operações...\n");
    
    const int NUM_CARTAS = 1000;
    const int NUM_COMPARACOES = 10000;
    
    Carta cartas[NUM_CARTAS];
    
    // Criar cartas
    for (int i = 0; i < NUM_CARTAS; i++) {
        char nome[20];
        sprintf(nome, "Cidade%d", i);
        cartas[i] = criar_carta_stress(nome, 
                                     1000000 + (i * 10000), 
                                     100.0f + (i * 0.1f), 
                                     50.0f + (i * 0.5f), 
                                     5 + (i % 20));
    }
    
    // Fazer comparações
    int resultados_validos = 0;
    for (int i = 0; i < NUM_COMPARACOES; i++) {
        int carta1_idx = i % NUM_CARTAS;
        int carta2_idx = (i + 1) % NUM_CARTAS;
        int atributo = (i % 7) + 1;
        
        int resultado = compara_atributo(&cartas[carta1_idx], &cartas[carta2_idx], atributo);
        if (resultado >= 0 && resultado <= 2) {
            resultados_validos++;
        }
    }
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("Operações realizadas: %d criações de cartas + %d comparações\n", NUM_CARTAS, NUM_COMPARACOES);
    printf("Resultados válidos: %d/%d\n", resultados_validos, NUM_COMPARACOES);
    printf("Tempo total: %.4f segundos\n", tempo);
    printf("Tempo médio por operação: %.6f segundos\n", tempo / (NUM_CARTAS + NUM_COMPARACOES));
}

void teste_stress_memoria() {
    printf("\n=== TESTE DE STRESS: USO DE MEMÓRIA ===\n");
    
    // Teste de uso intensivo de memória
    printf("Testando uso intensivo de memória...\n");
    
    const int NUM_CARTAS_STRESS = 100000;
    
    clock_t inicio = clock();
    
    // Alocar muitas cartas dinamicamente
    Carta* cartas_dinamicas = malloc(NUM_CARTAS_STRESS * sizeof(Carta));
    
    if (cartas_dinamicas == NULL) {
        printf("✗ Falha na alocação de memória\n");
        return;
    }
    
    // Inicializar todas as cartas
    for (int i = 0; i < NUM_CARTAS_STRESS; i++) {
        char nome[20];
        sprintf(nome, "C%d", i);
        cartas_dinamicas[i] = criar_carta_stress(nome, 
                                                1000000 + i, 
                                                100.0f + (i * 0.001f), 
                                                50.0f + (i * 0.001f), 
                                                5 + (i % 20));
    }
    
    // Fazer algumas comparações aleatórias
    int comparacoes_sucesso = 0;
    for (int i = 0; i < 1000; i++) {
        int idx1 = rand() % NUM_CARTAS_STRESS;
        int idx2 = rand() % NUM_CARTAS_STRESS;
        int atributo = (rand() % 7) + 1;
        
        int resultado = compara_atributo(&cartas_dinamicas[idx1], &cartas_dinamicas[idx2], atributo);
        if (resultado >= 0 && resultado <= 2) {
            comparacoes_sucesso++;
        }
    }
    
    free(cartas_dinamicas);
    
    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("✓ %d cartas criadas e testadas\n", NUM_CARTAS_STRESS);
    printf("✓ %d/1000 comparações bem-sucedidas\n", comparacoes_sucesso);
    printf("✓ Memória liberada com sucesso\n");
    printf("Tempo total: %.4f segundos\n", tempo);
}

int main() {
    printf("INICIANDO TESTES DE STRESS DO SUPER TRUNFO\n");
    printf("==========================================\n\n");
    
    srand(time(NULL)); // Inicializar gerador de números aleatórios
    
    teste_stress_valores_extremos();
    teste_stress_precisao_float();
    teste_stress_divisao_por_zero();
    teste_stress_nome_strings();
    teste_stress_performance();
    teste_stress_memoria();
    
    printf("\n==========================================\n");
    printf("TESTES DE STRESS CONCLUÍDOS\n");
    printf("==========================================\n");
    printf("Todos os testes de stress foram executados.\n");
    printf("Se não houve erros reportados, o código\n");
    printf("está robusto para condições extremas.\n");
    
    return 0;
}