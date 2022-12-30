# sb-trab1
trabalho 1 da disciplina de Software Básico da UnB 2022.2

## instruções para executar o montador

Primeiramente você deve estar na pasta ```/src``` do projeto.

Use o comando:

``` make montador ```

Após isso, use o comando:

``` ./montador -x program ```

Em que "x" tem que ser p, m ou o e "program" é o nome do código fonte sem a extensão asm.

## duvidas da equipe
~~preguiça de abrir uma issue ou coisa assim, bem mais facil usar o readme~~

.as extensões vão ser maiúsculas ou minúsculas? ex: .ASM ou .asm ou não faz diferença?

## quadro kanban? backlog? sla

**refatorei os dois primeiros codigos, ficou bem mais claro, falta fazer testes para verificar se eles estao corretos, mas ja corrigi o que tava faltando neles**

**comecano o proc_obj de uma forma mais recursiva (desisti, fui full procedural mas usei classe)**

**terminei bem por cima o montador, precisa melhorar a verificação de erros**

## requisitos

- ~~Aceitar maiúsculas e minúsculas (insensitive case)~~

- ~~A diretiva CONST deve aceitar positivos, negativos e hexa no formato 0X (no arquivo de saida OBJ tudo deve estar em 
decimal)~~ -> fiz mas tem que testar

- ~~O comando COPY deve separar os argumentos por "," SEM espaço~~

- ~~Desconsiderar todos os espaços, tabulações ou enter desnecessários~~ ja fazemos isso no pre processamento

- ~~Pode dar rótulo seguido de dois pontos e ENTER. O rótulo é considerado como da linha seguinte~~ -> fiz mas tem que testar ver se ta certo

- ~~SPACE pode aceitar argumento. Logo é possível fazer rótulos como X+2 (sem espaços)~~ -> fiz mas tem que testar

- ~~Aceitar comentário em qualquer parte do código iniciado por ; (o comentário deve ser removido no pré-processamento de EQU e IF)~~

## identificação de erros

- Dois rótulos na mesma linha -> fiz, ta localizado no construtor da linha, tenho que arrumar e testar

- Rótulo não definido -> fiz, ta na segunda passagem, criar caso de teste para isso

- Dado não definido -> mesma situacao da opçao acima

- Quantidade de argumentos errada -> mesma coisa acima

- Seção TEXT faltante -> mesma coisa acima

- Instrução ou diretiva inexistente -> mesma coisa acima

- Erros léxicos (caracteres especiais ou número iniciando nos rótulos) -> feito, mas tem q testar

**OBS: devemos indicar se o erro é LÉXICO, SINTÁTICO OU SEMÂNTICO**

## notas

- lembrar que section é uma diretiva

## aqui descrever algumas características do nosso compilador

- nas definições de macro, definimos os argumentos por virgula

- mas na chamada de macro não tem virgula