# xadrez_graduation_work
Meu segundo trabalho da faculdade de engenharia de software. Aqui desenvolvi um programa em C que mostra a movimentação de peças de xadrez, mas não um xadrez completamente funcional.

# Simulador de Movimentos de Xadrez

## Descrição do Projeto

Este projeto implementa a simulação de movimentos de peças de xadrez usando técnicas avançadas de programação em C:

- **Torre**: Movimento recursivo horizontal/vertical
- **Bispo**: Movimento recursivo e com loops aninhados diagonais
- **Rainha**: Movimento recursivo combinando Torre e Bispo
- **Cavalo**: Movimento com loops complexos em formato "L" (2 casas para cima, 1 para direita)

## Estrutura do Projeto

- `xadrez.h` - Arquivo de cabeçalho com protótipos e definições
- `xadrez.c` - Implementação principal do programa
- `Makefile` - Arquivo para compilação automatizada
- `.gitignore` - Configuração para ignorar arquivos compilados

## Como Compilar e Executar

### Usando Makefile (Recomendado)

```bash
# Compilar o programa
make

# Executar o programa
make run

# Limpar arquivos compilados
make clean

# Recompilar do zero
make rebuild