# ProcessFlow

O ProcessFlow é um orquestrador de processos desenvolvido em C 

O programa permite cadastrar tarefas e executá-las de diferentes formas, utilizando processos filhos com fork() e exec().

## Funcionalidades

- Cadastro de tarefas com task
- Execução simples com run
- Execução sequencial
- Execução paralela
- Comunicação entre processos utilizando pipe
- Redirecionamento de entrada com input
- Redirecionamento de saída com output
- Escrita no final de arquivos com append
- Alteração do diretório de trabalho com workdir
- Execução em background com start
- Listagem de jobs com jobs
- Espera de um job específico com wait
- Execução de comandos através de arquivo workflow

## Arquivos

### processflow.c

Arquivo principal do projetocom a implementação do ProcessFlow.

### Makefile

Utilizado para compilar, executar e limpar os arquivosdo projeto.

## Compilação


- make

O executável gerado será:


- ./processflow


Para remover os arquivos compilados:

make clean


## Execução

### Modo interativo


./processflow


Exemplo:


- processflow> task listar /bin/ls -l

- processflow> run listar

- processflow> exit


Também é possível executar:

- make run

## Workflow

O ProcessFlow também pode receber um arquivo com os comandos a serem processados.

./processflow teste.pf


Exemplo de teste.pf:

task ola /bin/echo Ola
run ola
exit


No modo workflow, cada linha do arquivo é exibida antes de ser processada e o prompt processflow> não é exibido.

## Exemplos de comandos

- Execução simples:


task ola /bin/echo Ola

run ola


- Execução sequencial:

run sequential tarefa1 tarefa2 tarefa3


- Execução paralela:


run parallel tarefa1 tarefa2 tarefa3


- Pipe:


run pipe tarefa1 tarefa2 tarefa3


- Redirecionamento:

input tarefa entrada.txt
output tarefa saida.txt
append tarefa saida.txt

- Diretório de trabalho:

workdir /tmp


- Background:

start tarefa

jobs

wait 1


## Testes

O programa pode ser testado no modo interativo:

./processflow


ou através de um workflow:

./processflow teste.pf


## Tecnologias utilizadas

- C
- GCC
- Make
- Linux / WSL

Principais chamadas de sistema utilizadas:

fork()
execv()
waitpid()
pipe()
dup2()
chdir()


## Sistema operacional

O projeto foi desenvolvido e testado em ambiente Linux.

## Autor

João mafra
