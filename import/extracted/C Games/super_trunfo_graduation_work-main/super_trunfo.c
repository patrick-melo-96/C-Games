#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definição do tipo Carta
typedef struct {
    char cidade[33];    // Nome da cidade (máx 32 caracteres + '\0')
    unsigned long int populacao;    // População em milhões
    float area;         // Área em km²
    float pib;         // PIB em bilhões
    int pontos_turisticos; // Quantidade de pontos turísticos
    float densidade_populacional; // População por km²
    float pib_per_capita; // PIB per capita
    float super_poder; // Soma dos atributos para o modo usuário
} Carta;

    // Declarações forward das funções
    float acessarAtributo(Carta* carta, int indice);
    void exibir_carta(Carta* carta, int numero_carta);
    void limpar_buffer_ate_enter();
    const char* nome_atributo(int atributo);
    int compara_atributo(Carta* c1, Carta* c2, int atributo);
    void listar_cidades(Carta* baralho[], int tamanho);
    void listar_cidades_disponiveis(Carta* baralho[], int tamanho, int carta_excluida);

//Identificadores de atributos - correspondem aos números mostrados no menu
#define IDX_POPULACAO 1      // opção 1 no menu
#define IDX_AREA 2           // opção 2 no menu
#define IDX_DENSIDADE 3      // opção 3 no menu
#define IDX_PIB 4            // opção 4 no menu
#define IDX_PIB_PER_CAPITA 5 // opção 5 no menu
#define IDX_PONTOS_TURISTICOS 6 // opção 6 no menu
#define IDX_SUPER_PODER 7    // opção 7 no menu

// Função para exibir o menu de atributos
void exibir_menu_atributos(bool atributos_disponiveis[]) {
    if (atributos_disponiveis[IDX_POPULACAO]) printf("%d. População\n", IDX_POPULACAO);
    if (atributos_disponiveis[IDX_AREA]) printf("%d. Área\n", IDX_AREA);
    if (atributos_disponiveis[IDX_DENSIDADE]) printf("%d. Densidade Populacional\n", IDX_DENSIDADE);
    if (atributos_disponiveis[IDX_PIB]) printf("%d. PIB\n", IDX_PIB);
    if (atributos_disponiveis[IDX_PIB_PER_CAPITA]) printf("%d. PIB per capita\n", IDX_PIB_PER_CAPITA);
    if (atributos_disponiveis[IDX_PONTOS_TURISTICOS]) printf("%d. Pontos Turísticos\n", IDX_PONTOS_TURISTICOS);
    if (atributos_disponiveis[IDX_SUPER_PODER]) printf("%d. Super Poder\n", IDX_SUPER_PODER);
}

// Função para verificar se um atributo é válido
bool atributo_valido(int atributo, bool atributos_disponiveis[]) {
    if (atributo < 1 || atributo > 7) return false;
    return atributos_disponiveis[atributo];
}

// Nome legível do atributo
const char* nome_atributo(int atributo) {
    switch (atributo) {
        case IDX_POPULACAO: return "População";
        case IDX_AREA: return "Área";
        case IDX_DENSIDADE: return "Densidade Populacional";
        case IDX_PIB: return "PIB";
        case IDX_PIB_PER_CAPITA: return "PIB per Capita";
        case IDX_PONTOS_TURISTICOS: return "Pontos Turísticos";
        case IDX_SUPER_PODER: return "Super Poder";
        default: return "Atributo";
    }
}

// Compara um atributo específico entre duas cartas
// Retorna: 1 se carta1 vence, 2 se carta2 vence, 0 se empate
int compara_atributo(Carta* c1, Carta* c2, int atributo) {
    float v1 = acessarAtributo(c1, atributo);
    float v2 = acessarAtributo(c2, atributo);
    bool menor_vence = (atributo == IDX_DENSIDADE);
    if (v1 == v2) return 0;
    if (menor_vence) return (v1 < v2) ? 1 : 2;
    return (v1 > v2) ? 1 : 2;
}

// Lista as cidades disponíveis
void listar_cidades(Carta* baralho[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d. %s\n", i + 1, baralho[i]->cidade);
    }
}

// Lista as cidades disponíveis excluindo uma carta já escolhida
void listar_cidades_disponiveis(Carta* baralho[], int tamanho, int carta_excluida) {
    for (int i = 0; i < tamanho; i++) {
        if (i != carta_excluida) {
            printf("%d. %s\n", i + 1, baralho[i]->cidade);
        }
    }
}

// Função para comparar cartas com base em dois atributos
void comparar_cartas_dois_atributos(Carta* carta1, Carta* carta2, int atributo1, int atributo2, int jogador1, int jogador2) {
    float valor1_atrib1 = acessarAtributo(carta1, atributo1);
    float valor2_atrib1 = acessarAtributo(carta2, atributo1);
    float valor1_atrib2 = acessarAtributo(carta1, atributo2);
    float valor2_atrib2 = acessarAtributo(carta2, atributo2);

    int pontos_carta1 = 0;
    int pontos_carta2 = 0;

    printf("\nComparando primeiro atributo:\n");
    const char* nome1 = nome_atributo(atributo1);
    switch (compara_atributo(carta1, carta2, atributo1)) {
        case 1: pontos_carta1++; printf("Carta %s do Jogador %d venceu carta %s do Jogador %d no atributo %s\n", carta1->cidade, jogador1, carta2->cidade, jogador2, nome1); break;
        case 2: pontos_carta2++; printf("Carta %s do Jogador %d venceu carta %s do Jogador %d no atributo %s\n", carta2->cidade, jogador2, carta1->cidade, jogador1, nome1); break;
        default: printf("Empate no atributo %s\n", nome1); break;
    }

    printf("\nComparando segundo atributo:\n");
    const char* nome2 = nome_atributo(atributo2);
    switch (compara_atributo(carta1, carta2, atributo2)) {
        case 1: pontos_carta1++; printf("Carta %s do Jogador %d venceu carta %s do Jogador %d no atributo %s\n", carta1->cidade, jogador1, carta2->cidade, jogador2, nome2); break;
        case 2: pontos_carta2++; printf("Carta %s do Jogador %d venceu carta %s do Jogador %d no atributo %s\n", carta2->cidade, jogador2, carta1->cidade, jogador1, nome2); break;
        default: printf("Empate no atributo %s\n", nome2); break;
    }

    float soma_carta1 = valor1_atrib1 + valor1_atrib2;
    float soma_carta2 = valor2_atrib1 + valor2_atrib2;

    printf("\nSoma dos atributos:\n");
    printf("%s: %.2f\n", carta1->cidade, soma_carta1);
    printf("%s: %.2f\n", carta2->cidade, soma_carta2);

    if (pontos_carta1 > pontos_carta2) {
        printf("\nCarta %s do Jogador %d venceu a rodada por ganhar mais atributos.\n", carta1->cidade, jogador1);
    } else if (pontos_carta2 > pontos_carta1) {
        printf("\nCarta %s do Jogador %d venceu a rodada por ganhar mais atributos.\n", carta2->cidade, jogador2);
    } else {
        if (soma_carta1 > soma_carta2) {
            printf("\nCarta %s do Jogador %d venceu a rodada por maior soma dos atributos.\n", carta1->cidade, jogador1);
        } else if (soma_carta2 > soma_carta1) {
            printf("\nCarta %s do Jogador %d venceu a rodada por maior soma dos atributos.\n", carta2->cidade, jogador2);
        } else {
            printf("\nEmpate total!\n");
        }
    }
}

// Função utilitária para limpar o buffer de entrada
// ou coletar a tecla Enter, recurso comum para ir
// para a próxima tela em jogos

void limpar_buffer_ate_enter() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para acessar atributos númericos das cartas
float acessarAtributo(Carta* carta, int indice) {
    // Os casos correspondem às opções do menu (1-7)
    switch (indice) {
        case IDX_POPULACAO: return carta->populacao;          // opção 1
        case IDX_AREA: return carta->area;                    // opção 2
        case IDX_DENSIDADE: return carta->densidade_populacional; // opção 3
        case IDX_PIB: return carta->pib;                      // opção 4
        case IDX_PIB_PER_CAPITA: return carta->pib_per_capita;   // opção 5
        case IDX_PONTOS_TURISTICOS: return (float)carta->pontos_turisticos; // opção 6
        case IDX_SUPER_PODER: return carta->super_poder;      // opção 7
        default: return -1.0f; // índice inválido ou não numérico
    }
}


// Função para exibir uma carta no formato solicitado
void exibir_carta(Carta* carta, int numero_carta) {
    printf("\nCarta %d:\n", numero_carta);
    printf("Nome da Cidade: %s\n", carta->cidade);
    printf("População: %lu habitantes\n", carta->populacao);
    printf("Área: %.2f km²\n", carta->area);
    printf("PIB: %.2f bilhões de reais\n", carta->pib);
    printf("Número de Pontos Turísticos: %d\n", carta->pontos_turisticos);
    printf("Densidade Populacional: %.2f hab/km²\n", carta->densidade_populacional);
    printf("PIB per Capita: %.2f reais\n", carta->pib_per_capita);
    printf("Super Poder: %.2f\n", carta->super_poder);
    printf("\nEssa é a carta que você deseja? (S/N): ");
}

// Função principal
int main(void) {
    // Telas do jogo
        typedef struct {
            char intro[1024];
            char menu_inicial[1024];
            char regras[1024];
            char sair[1024];
            char creditos[1024];
        } Tela;

        Tela telas = {
        .intro =
            "\n========================================\n"
            "          JOGO SUPER TRUNFO            \n"
            "========================================\n\n"
            "Bem-vindo ao Super Trunfo!\n\n"
            "Pressione Enter para iniciar o jogo...\n",
        .menu_inicial =
            "\n========================================\n"
            "                MENU                   \n"
            "\n"
            "1. Iniciar Jogo\n"
            "2. Ver regras\n"
            "3. Créditos\n"
            "4. Sair\n"
            "\n"
            "Escolha uma opção digitando o número correspondente (1-4): \n",

        .regras =
            "\n========================================\n"
            "                REGRAS                 \n"
            "========================================\n"
            "1. Cada jogador escolhe uma carta de cidade dentre as disponíveis.\n"
            "2. Você escolhe dois atributos diferentes para comparar.\n"
            "3. Cada atributo é comparado separadamente.\n"
            "4. A carta que vencer em mais atributos ganha.\n"
            "5. Em caso de empate, vence a maior soma dos atributos.\n"
            "6. Exceção: Para Densidade Populacional, vence o MENOR valor.\n"
            "========================================\n"
            "Digite 1 e clique Enter para voltar ao menu inicial:\n",

        .sair =
            "\n========================================\n"
            "          SAINDO DO JOGO               \n"
            "========================================\n"
            "Obrigado por jogar! Até a próxima!\n",
        .creditos =
            "\n========================================\n"
            "                CRÉDITOS                \n"
            "========================================\n"
            "Desenvolvido por:\n"
            " - Patrick Melo\n"
            "========================================\n"
            "Aperte Enter para voltar ao menu inicial...\n"
        };

    // Cartas (instâncias de `Carta` declaradas abaixo)
    Carta carta_Manaus;
    strncpy(carta_Manaus.cidade, "Manaus", sizeof(carta_Manaus.cidade) - 1);
    carta_Manaus.cidade[sizeof(carta_Manaus.cidade)-1] = '\0';
    carta_Manaus.populacao = 2000000;
    carta_Manaus.area = 11401.0;
    carta_Manaus.pib = 74.0;
    carta_Manaus.pontos_turisticos = 8;
    carta_Manaus.densidade_populacional = (carta_Manaus.area != 0.0f) ? 
        (carta_Manaus.populacao / carta_Manaus.area) : 0.0f;
    carta_Manaus.pib_per_capita = (carta_Manaus.populacao != 0.0f) ?
        ((carta_Manaus.pib * 1000000000) / (carta_Manaus.populacao)) : 0.0f;
    carta_Manaus.super_poder = carta_Manaus.populacao + carta_Manaus.area + carta_Manaus.pib +
                               carta_Manaus.pontos_turisticos + carta_Manaus.pib_per_capita +
                               (carta_Manaus.densidade_populacional != 0.0f ? 1.0f / carta_Manaus.densidade_populacional : 0.0f);

    Carta carta_SaoPaulo;
    strncpy(carta_SaoPaulo.cidade, "Sao Paulo", sizeof(carta_SaoPaulo.cidade) - 1);
    carta_SaoPaulo.cidade[sizeof(carta_SaoPaulo.cidade)-1] = '\0';
    carta_SaoPaulo.populacao = 12000000;
    carta_SaoPaulo.area = 1521.0;
    carta_SaoPaulo.pib = 443.0;
    carta_SaoPaulo.pontos_turisticos = 15;
    carta_SaoPaulo.densidade_populacional = (carta_SaoPaulo.area != 0.0f) ? 
        (carta_SaoPaulo.populacao / carta_SaoPaulo.area) : 0.0f;
    carta_SaoPaulo.pib_per_capita = (carta_SaoPaulo.populacao != 0.0f) ?
        ((carta_SaoPaulo.pib * 1000000000) / (carta_SaoPaulo.populacao)) : 0.0f;
    carta_SaoPaulo.super_poder = carta_SaoPaulo.populacao + carta_SaoPaulo.area + carta_SaoPaulo.pib +
                                 carta_SaoPaulo.pontos_turisticos + carta_SaoPaulo.pib_per_capita +
                                 (carta_SaoPaulo.densidade_populacional != 0.0f ? 1.0f / carta_SaoPaulo.densidade_populacional : 0.0f);

    Carta carta_RioDeJaneiro;
    strncpy(carta_RioDeJaneiro.cidade, "Rio de Janeiro", sizeof(carta_RioDeJaneiro.cidade) - 1);
    carta_RioDeJaneiro.cidade[sizeof(carta_RioDeJaneiro.cidade)-1] = '\0';
    carta_RioDeJaneiro.populacao = 6000000;
    carta_RioDeJaneiro.area = 1182.0;
    carta_RioDeJaneiro.pib = 200.0;
    carta_RioDeJaneiro.pontos_turisticos = 10;
    carta_RioDeJaneiro.densidade_populacional = (carta_RioDeJaneiro.area != 0.0f) ? 
        (carta_RioDeJaneiro.populacao / carta_RioDeJaneiro.area) : 0.0f;
    carta_RioDeJaneiro.pib_per_capita = (carta_RioDeJaneiro.populacao != 0.0f) ?
        ((carta_RioDeJaneiro.pib * 1000000000) / (carta_RioDeJaneiro.populacao)) : 0.0f;
    carta_RioDeJaneiro.super_poder = carta_RioDeJaneiro.populacao + carta_RioDeJaneiro.area + carta_RioDeJaneiro.pib +
                                     carta_RioDeJaneiro.pontos_turisticos + carta_RioDeJaneiro.pib_per_capita +
                                     (carta_RioDeJaneiro.densidade_populacional != 0.0f ? 1.0f / carta_RioDeJaneiro.densidade_populacional : 0.0f);

    Carta carta_Brasilia;
    strncpy(carta_Brasilia.cidade, "Brasilia", sizeof(carta_Brasilia.cidade) - 1);
    carta_Brasilia.cidade[sizeof(carta_Brasilia.cidade)-1] = '\0';
    carta_Brasilia.populacao = 3000000;
    carta_Brasilia.area = 5802.0;
    carta_Brasilia.pib = 220.0;
    carta_Brasilia.pontos_turisticos = 5;
    carta_Brasilia.densidade_populacional = (carta_Brasilia.area != 0.0f) ? 
        (carta_Brasilia.populacao / carta_Brasilia.area) : 0.0f;
    carta_Brasilia.pib_per_capita = (carta_Brasilia.populacao != 0.0f) ?
        ((carta_Brasilia.pib * 1000000000) / (carta_Brasilia.populacao)) : 0.0f;
    carta_Brasilia.super_poder = carta_Brasilia.populacao + carta_Brasilia.area + carta_Brasilia.pib +
                                 carta_Brasilia.pontos_turisticos + carta_Brasilia.pib_per_capita +
                                 (carta_Brasilia.densidade_populacional != 0.0f ? 1.0f / carta_Brasilia.densidade_populacional : 0.0f);

    Carta carta_Salvador;
    strncpy(carta_Salvador.cidade, "Salvador", sizeof(carta_Salvador.cidade) - 1);
    carta_Salvador.cidade[sizeof(carta_Salvador.cidade)-1] = '\0';
    carta_Salvador.populacao = 3000000;
    carta_Salvador.area = 693.0;
    carta_Salvador.pib = 130.0;
    carta_Salvador.pontos_turisticos = 7;
    carta_Salvador.densidade_populacional = (carta_Salvador.area != 0.0f) ? 
        (carta_Salvador.populacao / carta_Salvador.area) : 0.0f;
    carta_Salvador.pib_per_capita = (carta_Salvador.populacao != 0.0f) ?
        ((carta_Salvador.pib * 1000000000) / (carta_Salvador.populacao)) : 0.0f;
    carta_Salvador.super_poder = carta_Salvador.populacao + carta_Salvador.area + carta_Salvador.pib +
                                 carta_Salvador.pontos_turisticos + carta_Salvador.pib_per_capita +
                                 (carta_Salvador.densidade_populacional != 0.0f ? 1.0f / carta_Salvador.densidade_populacional : 0.0f);

    Carta carta_Fortaleza;
    strncpy(carta_Fortaleza.cidade, "Fortaleza", sizeof(carta_Fortaleza.cidade) - 1);
    carta_Fortaleza.cidade[sizeof(carta_Fortaleza.cidade)-1] = '\0';
    carta_Fortaleza.populacao = 2000000;
    carta_Fortaleza.area = 313.0;
    carta_Fortaleza.pib = 90.0;
    carta_Fortaleza.pontos_turisticos = 6;
    carta_Fortaleza.densidade_populacional = (carta_Fortaleza.area != 0.0f) ? 
        (carta_Fortaleza.populacao / carta_Fortaleza.area) : 0.0f;
    carta_Fortaleza.pib_per_capita = (carta_Fortaleza.populacao != 0.0f) ?
        ((carta_Fortaleza.pib * 1000000000) / (carta_Fortaleza.populacao)) : 0.0f;
    carta_Fortaleza.super_poder = carta_Fortaleza.populacao + carta_Fortaleza.area + carta_Fortaleza.pib +
                                  carta_Fortaleza.pontos_turisticos + carta_Fortaleza.pib_per_capita +
                                  (carta_Fortaleza.densidade_populacional != 0.0f ? 1.0f / carta_Fortaleza.densidade_populacional : 0.0f);

    Carta carta_BeloHorizonte;
    strncpy(carta_BeloHorizonte.cidade, "Belo Horizonte", sizeof(carta_BeloHorizonte.cidade) - 1);
    carta_BeloHorizonte.cidade[sizeof(carta_BeloHorizonte.cidade)-1] = '\0';
    carta_BeloHorizonte.populacao = 2000000;
    carta_BeloHorizonte.area = 331.0;
    carta_BeloHorizonte.pib = 110.0;
    carta_BeloHorizonte.pontos_turisticos = 4;
    carta_BeloHorizonte.densidade_populacional = (carta_BeloHorizonte.area != 0.0f) ? 
        (carta_BeloHorizonte.populacao / carta_BeloHorizonte.area) : 0.0f;
    carta_BeloHorizonte.pib_per_capita = (carta_BeloHorizonte.populacao != 0.0f) ?
        ((carta_BeloHorizonte.pib * 1000000000) / (carta_BeloHorizonte.populacao)) : 0.0f;
    carta_BeloHorizonte.super_poder = carta_BeloHorizonte.populacao + carta_BeloHorizonte.area + carta_BeloHorizonte.pib +
                                      carta_BeloHorizonte.pontos_turisticos + carta_BeloHorizonte.pib_per_capita +
                                      (carta_BeloHorizonte.densidade_populacional != 0.0f ? 1.0f / carta_BeloHorizonte.densidade_populacional : 0.0f);

    Carta carta_Curitiba;
    strncpy(carta_Curitiba.cidade, "Curitiba", sizeof(carta_Curitiba.cidade) - 1);
    carta_Curitiba.cidade[sizeof(carta_Curitiba.cidade)-1] = '\0';
    carta_Curitiba.populacao = 2000000;
    carta_Curitiba.area = 435.0;
    carta_Curitiba.pib = 150.0;
    carta_Curitiba.pontos_turisticos = 3;
    carta_Curitiba.densidade_populacional = (carta_Curitiba.area != 0.0f) ? 
        (carta_Curitiba.populacao / carta_Curitiba.area) : 0.0f;
    carta_Curitiba.pib_per_capita = (carta_Curitiba.populacao != 0.0f) ?
        ((carta_Curitiba.pib * 1000000000) / (carta_Curitiba.populacao)) : 0.0f;
    carta_Curitiba.super_poder = carta_Curitiba.populacao + carta_Curitiba.area + carta_Curitiba.pib +
                                 carta_Curitiba.pontos_turisticos + carta_Curitiba.pib_per_capita +
                                 (carta_Curitiba.densidade_populacional != 0.0f ? 1.0f / carta_Curitiba.densidade_populacional : 0.0f);

    // Baralho para facilitar acesso por índice
    Carta* baralho[8] = {
        &carta_Manaus,
        &carta_SaoPaulo,
        &carta_RioDeJaneiro,
        &carta_Brasilia,
        &carta_Salvador,
        &carta_Fortaleza,
        &carta_BeloHorizonte,
        &carta_Curitiba
    };

    //Jogo
        int tela_atual = 0; 
        while (1) {
            switch (tela_atual) {
                // 0 = Intro 
                // 1 = Menu inicial
                // 2 = Seleção de modo de jogo 
                // 3 = Regras
                // 4 = Sair
                
                case 0: { // Intro
                    printf("%s", telas.intro);
                    limpar_buffer_ate_enter();
                    tela_atual = 1;
                    break;
                }
                case 1: { // Menu Inicial
                    printf("%s", telas.menu_inicial);
                    int opcao;
                    while (1) {
                        if (scanf("%d", &opcao) != 1) { // Verifica se a entrada é um número
                            limpar_buffer_ate_enter();
                            printf("Opção inválida. Por favor, escolha 1, 2, 3 ou 4:\n");
                            continue;
                        }
                        limpar_buffer_ate_enter(); // Limpa o \n que fica no buffer após scanf
                        switch (opcao) {
                            case 1:{
                                tela_atual = 2; // Iniciar jogo direto
                                break;
                            }
                            case 2:{ // Regras
                                tela_atual = 3;
                                break;
                            }
                            case 3:{ // Créditos
                                tela_atual = 5;
                                break;
                            }
                            case 4:{ // Sair
                                tela_atual = 4;
                                break;
                            }
                            default:{
                                printf("Opção inválida. Por favor, escolha 1, 2, 3 ou 4:\n");
                                continue;
                            }
                        }
                        break; // sai do while
                    }
                    break; // sai do case 1
                }
                case 2: { // Jogo - escolha de cartas e atributos
                    printf("\n========================================\n");
                    printf("          SUPER TRUNFO - JOGO          \n");
                    printf("========================================\n\n");
                    // Escolha das cartas primeiro
                    int carta_escolhida_jogador_1 = -1;
                    printf("\nEscolha a carta para o Jogador 1:\n");
                    listar_cidades(baralho, 8);
                    
                    while (1) {
                        if (scanf("%d", &carta_escolhida_jogador_1) != 1) {
                            limpar_buffer_ate_enter();
                            printf("Entrada inválida. Por favor, escolha um número entre 1 e 8:\n");
                            continue;
                        }
                        if (carta_escolhida_jogador_1 >= 1 && carta_escolhida_jogador_1 <= 8) {
                            carta_escolhida_jogador_1--; // Ajusta para índice 0-7
                            // Mostra os detalhes da carta escolhida
                            exibir_carta(baralho[carta_escolhida_jogador_1], 1);
                            char confirmacao;
                            if (scanf(" %c", &confirmacao) != 1) {
                                printf("\nEntrada encerrada.\n");
                                return EXIT_FAILURE;
                            }
                            if (confirmacao == 'S' || confirmacao == 's') {
                                break;
                            }
                            printf("\nEscolha outra carta:\n");
                            listar_cidades(baralho, 8);
                            continue;
                        }
                        printf("Carta inválida. Por favor, escolha um número entre 1 e 8.\n");
                    }

                    int carta_escolhida_jogador_2 = -1;
                    printf("\nEscolha a carta para o Jogador 2:\n");
                    listar_cidades_disponiveis(baralho, 8, carta_escolhida_jogador_1);
                    while (1) {
                        if (scanf("%d", &carta_escolhida_jogador_2) != 1) {
                            limpar_buffer_ate_enter();
                            printf("Entrada inválida. Por favor, escolha um número entre 1 e 8:\n");
                            continue;
                        }
                        if (carta_escolhida_jogador_2 >= 1 && carta_escolhida_jogador_2 <= 8) {
                            carta_escolhida_jogador_2--; // Ajusta para 0-7
                            if (carta_escolhida_jogador_2 == carta_escolhida_jogador_1) {
                                printf("Essa carta já foi escolhida pelo Jogador 1. Escolha outra carta:\n");
                                continue;
                            }
                            // Mostra os detalhes da carta escolhida
                            exibir_carta(baralho[carta_escolhida_jogador_2], 2);
                            char confirmacao;
                            if (scanf(" %c", &confirmacao) != 1) {
                                printf("\nEntrada encerrada.\n");
                                return EXIT_FAILURE;
                            }
                            if (confirmacao == 'S' || confirmacao == 's') {
                                break;
                            }
                            printf("\nEscolha outra carta:\n");
                            listar_cidades_disponiveis(baralho, 8, carta_escolhida_jogador_1);
                            continue;
                        }
                        printf("Carta inválida ou já escolhida. Por favor, escolha outra carta.\n");
                    }

                    // Mapear os índices para os ponteiros das cartas via baralho
                    Carta *selA = baralho[carta_escolhida_jogador_1];
                    Carta *selB = baralho[carta_escolhida_jogador_2];

                    // Agora escolher os atributos
                    bool atributos_disponiveis[8] = {false, true, true, true, true, true, true, true};
                    int atributo1 = -1;
                    while (1) {
                        printf("\nEscolha o primeiro atributo para comparação:\n");
                        exibir_menu_atributos(atributos_disponiveis);
                        printf("\nDigite o número do atributo (1-7): ");
                        if (scanf("%d", &atributo1) != 1) {
                            limpar_buffer_ate_enter();
                            printf("Entrada inválida. Por favor, escolha um número entre 1 e 7.\n");
                            continue;
                        }
                        if (atributo_valido(atributo1, atributos_disponiveis)) {
                            break;
                        }
                        printf("Atributo inválido. Por favor, escolha um dos atributos disponíveis.\n");
                    }
                    atributos_disponiveis[atributo1] = false;

                    int atributo2 = -1;
                    while (1) {
                        printf("\nEscolha o segundo atributo para comparação:\n");
                        exibir_menu_atributos(atributos_disponiveis);
                        printf("\nDigite o número do atributo (1-7): ");
                        if (scanf("%d", &atributo2) != 1) {
                            limpar_buffer_ate_enter();
                            printf("Entrada inválida. Por favor, escolha um número entre 1 e 7.\n");
                            continue;
                        }
                        if (atributo_valido(atributo2, atributos_disponiveis)) {
                            break;
                        }
                        printf("Atributo inválido ou já escolhido. Por favor, escolha outro atributo.\n");
                    }

                    printf("\nComparando as cartas com os atributos escolhidos:\n");
                    comparar_cartas_dois_atributos(selA, selB, atributo1, atributo2, 1, 2);

                    printf("\nPressione Enter para continuar...");
                    limpar_buffer_ate_enter();
                    getchar();
                    tela_atual = 5; // Vai para os créditos
                    break;
                }
                case 3: { // Regras
                    printf("%s", telas.regras);
                    int opcao = 0;
                    while (1) {
                        if (scanf("%d", &opcao) != 1) {
                            limpar_buffer_ate_enter();
                            printf("\nPor favor digite 1 para voltar ao menu inicial:\n");
                            continue;
                        }
                        limpar_buffer_ate_enter();
                        if (opcao == 1) break;
                        printf("\nPor favor digite 1 para voltar ao menu inicial:\n");
                    }
                    tela_atual = 1;
                    break;
                 }
                case 4: { // Sair
                    printf("%s", telas.sair);
                    printf("Saindo do jogo. Até a próxima!\n");
                    return 0;
                }
                case 5: { // Créditos
                    printf("%s", telas.creditos);
                    limpar_buffer_ate_enter();
                    tela_atual = 1;
                    break;
                }
                default: { // Erro: tela desconhecida
                    printf("Erro: Tela desconhecida. Voltando ao menu inicial.\n");
                    tela_atual = 1;
                    break;
                } 
            }
        }
       return 0;
}