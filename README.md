# sb-trab1
trabalho 1 da disciplina de Software Básico da UnB 2022.2

## instruções para executar o montador

Primeiramente você deve estar na pasta ```/src``` do projeto.

Também é necessário ter o Make instalado

Após ter feito essas verificações passos, use o comando:

``` make montador ```

Após isso, use o comando:

``` ./montador -x program ```

Em que "x" tem que ser p, m ou o e "program" é o nome do código fonte sem a extensão asm.

Primeiro deve-se usar -p e -m para então usar o -o, caso o contrário o programa não encontrará o arquivo para compilar

## requisitos

- Aceitar maiúsculas e minúsculas (insensitive case)

- A diretiva CONST deve aceitar positivos, negativos e hexa no formato 0X (no arquivo de saida OBJ tudo deve estar em 
decimal)

- O comando COPY deve separar os argumentos por "," SEM espaço

- Desconsiderar todos os espaços, tabulações ou enter desnecessários

- Pode dar rótulo seguido de dois pontos e ENTER. O rótulo é considerado como da linha seguinte

- SPACE pode aceitar argumento. Logo é possível fazer rótulos como X+2 (sem espaços)

- Aceitar comentário em qualquer parte do código iniciado por ; (o comentário deve ser removido no pré-processamento de EQU e IF)

## identificação de erros

- Dois rótulos na mesma linha

- Rótulo não definido 

- Dado não definido 
- Quantidade de argumentos errada

- Seção TEXT faltante

- Instrução ou diretiva inexistente 

- Erros léxicos (caracteres especiais ou número iniciando nos rótulos)

## Algumas características do compilador

- nas definições de macro, definimos os argumentos por virgula

- mas na chamada de macro não tem virgula

- pela nossa interpretação da linguagem, estamos considerando o comando de multiplicação como sendo "mul" como está na tabela de slide, e não "mult" como usado no exemplo do trabalho, como acreditamos que isso não faça muita diferença para o objetivo final do trabalho, mantivemos desse jeito. Porém se for melhor para rodar testes, é bastante simples mudar isso. (Aliás, será que a gente deixa implementado para aceitar os dois comandos?)