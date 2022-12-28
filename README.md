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

**aparentemente terminei o "-p" falta testar mais casos, mas pelo o que eu vi ja ta tirando os comentários e pré processando os EQU e IF.**

~~no pre_proc falta substituir toda ocorrencia de EQU no codigo -> aparentemente resolvido~~

**no proc_macro falta consertar para ver se os argumentos da macro vao ser recebidos por virgula ou espaço (atualmente funciona com espaço)**

**tanto no pre_proc quanto no proc_macro a gente pode ponderar refatorar de forma recursiva**

**comecano o proc_obj de uma forma mais recursiva (desisti, fui full procedural mas usei classe)**

**terminei bem por cima o montador, precisa melhorar a verificação de erros**
