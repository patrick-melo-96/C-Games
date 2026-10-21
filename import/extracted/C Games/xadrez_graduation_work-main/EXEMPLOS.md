# Exemplos de Movimentação das Peças

Este documento mostra exemplos de como cada peça se movimenta no tabuleiro.

## Torre (Rook)

A Torre move-se horizontalmente e verticalmente, podendo percorrer qualquer número de casas em linha reta.

**Exemplo: Torre em d4**

```
  a b c d e f g h 
8 . . . * . . . . 8
7 . . . * . . . . 7
6 . . . * . . . . 6
5 . . . * . . . . 5
4 * * * T * * * * 4
3 . . . * . . . . 3
2 . . . * . . . . 2
1 . . . * . . . . 1
  a b c d e f g h 
```

Legenda:
- `T` = Torre
- `*` = Movimento possível
- `.` = Casa vazia

## Bispo (Bishop)

O Bispo move-se diagonalmente, podendo percorrer qualquer número de casas nas diagonais.

**Exemplo: Bispo em e5**

```
  a b c d e f g h 
8 . * . . . . . * 8
7 . . * . . . * . 7
6 . . . * . * . . 6
5 . . . . B . . . 5
4 . . . * . * . . 4
3 . . * . . . * . 3
2 . * . . . . . * 2
1 * . . . . . . . 1
  a b c d e f g h 
```

Legenda:
- `B` = Bispo
- `*` = Movimento possível
- `.` = Casa vazia

## Rainha (Queen)

A Rainha é a peça mais poderosa, combinando os movimentos da Torre e do Bispo. Pode mover-se horizontalmente, verticalmente e diagonalmente.

**Exemplo: Rainha em d4**

```
  a b c d e f g h 
8 . . . * . . . * 8
7 * . . * . . * . 7
6 . * . * . * . . 6
5 . . * * * . . . 5
4 * * * R * * * * 4
3 . . * * * . . . 3
2 . * . * . * . . 2
1 * . . * . . * . 1
  a b c d e f g h 
```

Legenda:
- `R` = Rainha
- `*` = Movimento possível
- `.` = Casa vazia

## Como Usar o Programa

1. Execute o programa: `./xadrez`
2. Escolha a peça que deseja demonstrar (1, 2 ou 3)
3. Digite a posição no formato algébrico (ex: e4, d5, a1)
4. O programa mostrará todos os movimentos possíveis
5. Você pode testar outras peças ou sair do programa (opção 0)

## Características do Sistema de Coordenadas

- **Colunas**: Representadas por letras de 'a' a 'h' (da esquerda para a direita)
- **Linhas**: Representadas por números de 1 a 8 (de baixo para cima)
- **Notação**: Sempre coluna + linha (ex: e4, d7, h1)

Este é o sistema padrão de notação algébrica usado no xadrez internacional.
