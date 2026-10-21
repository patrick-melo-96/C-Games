# Super Trunfo (C) — Documentação do Código

## Visão geral

Este projeto implementa, em C, uma versão simplificada do jogo Super Trunfo usando cartas de cidades brasileiras. O jogo roda no terminal e permite que dois jogadores escolham cartas e comparem dois atributos para decidir o vencedor da rodada.

O código principal está em `super_trunfo.c` e há dois programas auxiliares de teste:
- `test_super_trunfo.c`: testes funcionais e de unidade dos cálculos e comparações.
- `test_stress.c`: testes de stress (extremos, performance, memória).
- `test_manual.sh`: roteiro para validação manual de entradas inválidas e fluxo de menus.

## Estrutura de dados principal

A estrutura `Carta` representa uma cidade com os seguintes atributos (valores e unidades entre parênteses):

```c
typedef struct {
    char cidade[33];                // Nome da cidade (até 32 chars + '\0')
    unsigned long int populacao;    // Habitantes (inteiro)
    float area;                     // Área em km²
    float pib;                      // PIB em bilhões de reais
    int pontos_turisticos;          // Quantidade de pontos turísticos
    float densidade_populacional;   // Habitantes por km² (derivado)
    float pib_per_capita;           // Reais por habitante (derivado)
    float super_poder;              // Soma ponderada (derivado)
} Carta;
```

Observações sobre campos derivados:
- `densidade_populacional = populacao / area` (se `area == 0`, fica 0.0 para evitar divisão por zero).
- `pib_per_capita = (pib * 1_000_000_000) / populacao` (se `populacao == 0`, fica 0.0).
- `super_poder = populacao + area + pib + pontos_turisticos + pib_per_capita + (area > 0 ? 1.0/densidade_populacional : 0.0)`.

## Índices dos atributos (menu)

Os atributos são referenciados por constantes numéricas, usadas no menu e nas funções de comparação:

- 1 — População (`IDX_POPULACAO`)
- 2 — Área (`IDX_AREA`)
- 3 — Densidade Populacional (`IDX_DENSIDADE`) [menor vence]
- 4 — PIB (`IDX_PIB`)
- 5 — PIB per capita (`IDX_PIB_PER_CAPITA`)
- 6 — Pontos Turísticos (`IDX_PONTOS_TURISTICOS`)
- 7 — Super Poder (`IDX_SUPER_PODER`)

Observação importante: apenas para Densidade Populacional (3) o menor valor vence; para os demais, o maior vence.

## Funções principais (API interna)

- `float acessarAtributo(Carta* carta, int indice)`: retorna o valor numérico do atributo indicado (1–7). Para índices inválidos, retorna `-1.0f`.
- `const char* nome_atributo(int atributo)`: retorna o nome legível do atributo.
- `int compara_atributo(Carta* c1, Carta* c2, int atributo)`: compara um atributo entre duas cartas; retorna `1` (carta1 vence), `2` (carta2 vence) ou `0` (empate). Implementa a regra "menor vence" para densidade.
- `void exibir_carta(Carta* carta, int numero_carta)`: imprime os campos da carta formatados (com unidades) e pede confirmação `S/N` no final.
- `void exibir_menu_atributos(bool atributos_disponiveis[])`: imprime apenas os atributos disponíveis (após bloquear o já escolhido).
- `bool atributo_valido(int atributo, bool atributos_disponiveis[])`: valida índice (1–7) e disponibilidade.
- `void listar_cidades(Carta* baralho[], int tamanho)`: lista nomes das cidades (1..N).
- `void listar_cidades_disponiveis(Carta* baralho[], int tamanho, int carta_excluida)`: lista todas exceto a já escolhida.
- `void comparar_cartas_dois_atributos(Carta* a, Carta* b, int atr1, int atr2, int jogador1, int jogador2)`: executa a comparação atributo a atributo, mostra vencendor por atributo, soma dos dois atributos e aplica desempate por soma (se necessário).
- `void limpar_buffer_ate_enter()`: consome o buffer de entrada até encontrar `\n` (usado para saneamento de entradas e pausas).

## Fluxo do programa (máquina de estados)

O `main` implementa um loop de telas controlado por `tela_atual`:

- Estado 0 — Intro: exibe a tela de boas-vindas e avança ao pressionar Enter.
- Estado 1 — Menu inicial: opções `1` Iniciar jogo, `2` Regras, `3` Créditos, `4` Sair.
- Estado 2 — Jogo: fluxo da partida:
  - Jogador 1 escolhe uma carta (1–8), visualiza os detalhes e confirma com `S`.
  - Jogador 2 escolhe uma carta diferente, visualiza e confirma com `S`.
  - Usuário escolhe dois atributos distintos (1–7). O menu bloqueia automaticamente o atributo já escolhido.
  - O programa compara as cartas primeiro por `atributo1`, depois por `atributo2`.
  - Critério de vitória:
    - Quem vencer em mais atributos ganha.
    - Em caso de 1×1, desempata pela maior soma dos dois atributos escolhidos.
    - Exceção: para Densidade Populacional, o menor valor vence.
  - Ao final, o programa volta para a tela de Créditos.
- Estado 3 — Regras: exibe as regras e retorna ao menu quando o usuário digita `1`.
- Estado 4 — Sair: exibe mensagem de saída e termina o programa.
- Estado 5 — Créditos: exibe créditos e retorna ao menu ao pressionar Enter.

Todas as entradas de menu e seleção são validadas; em caso de entrada inválida, a leitura é saneada e a pergunta é repetida.

## Baralho e dados

O baralho é composto por 8 cartas de cidades (Manaus, São Paulo, Rio de Janeiro, Brasília, Salvador, Fortaleza, Belo Horizonte, Curitiba). Cada carta tem seus valores e todos os derivados são calculados na inicialização, com proteções contra divisão por zero.

## Como compilar e executar

Você pode usar o GCC disponível no ambiente. Exemplos de execução (opcionais):

```bash
# Compilar o jogo
gcc -o super_trunfo super_trunfo.c -lm

# Executar
./super_trunfo
```

### Testes funcionais (unidade)

```bash
# Compilar e executar testes
gcc -o test_super_trunfo test_super_trunfo.c -lm
./test_super_trunfo
```

Exemplo de resumo observado:
- Total de testes executados: 49
- Testes que passaram: 49
- Testes que falharam: 0
- Taxa de sucesso: 100.00%

### Testes de stress

```bash
# Compilar e executar testes de stress
gcc -O2 -o test_stress test_stress.c -lm
./test_stress
```

Os testes de stress cobrem: valores extremos (mín./máx.), divisões por zero, precisão de floats, strings de tamanho limite/especiais, performance (10k comparações) e uso de memória (100k cartas alocadas e liberadas).

### Testes manuais (interação)

```bash
# Tornar o script executável e seguir o roteiro
chmod +x test_manual.sh
./test_manual.sh
```

O script orienta testes de entradas inválidas em menus, seleção de cartas, confirmação e atributos.

## Regras do jogo (resumo)

1. Cada jogador escolhe uma carta de cidade entre as disponíveis.
2. O usuário escolhe dois atributos diferentes para comparar entre as cartas.
3. A comparação é feita atributo a atributo; quem vencer mais atributos, vence a rodada.
4. Em caso de empate, vence a maior soma dos dois atributos escolhidos.
5. Exceção: para Densidade Populacional, vence o MENOR valor.

## Considerações e limitações

- O conjunto de cartas é fixo no código (8 cidades). Não há persistência externa.
- Unidades: população em habitantes, área em km², PIB em bilhões de reais. O PIB per capita é calculado em reais por habitante.
- A regra de desempate usa a soma direta dos dois atributos escolhidos (sem normalização). A escolha de atributos muito distintos em escala pode influenciar o resultado.
- Entradas inválidas são tratadas por loops de validação e limpeza de buffer; ainda assim, por ser CLI, exige interação atenta (confirmar com `S` para prosseguir na escolha de cartas).

## Organização dos arquivos

- `super_trunfo.c`: implementação do jogo (lógica, dados e UI de terminal).
- `test_super_trunfo.c`: testes de unidade/funcionais dos cálculos e comparações.
- `test_stress.c`: testes de stress (extremos, precisão, performance, memória).
- `test_manual.sh`: roteiro de testes manuais interativos.
- `compilacoes/`: pasta reservada para artefatos de build (se desejar).
