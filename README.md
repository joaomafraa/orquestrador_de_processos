# orquestrador_de_processos
...

# imersao
## Fluxo do programa
processflow->le o comando->identifica o comando->executa a logica->volta a esperar para outro


## FUNCOES
* fork ()

ele cria o processo filho e ele retorna o pid do filho caso o processo seja o pai e retorna 0 caso ele ja esteja no pid do filho
* task 
ele nao executa nada serve para cadastrar as tarefas

processflow> task nome programa argumentos...

ex: task listar /bin/ls -l no processflow é 

Nome da tarefa: listar
Programa: /bin/ls
Argumento: -l

* run

apos cadastro a tarefa deve ser executada com run [nome] 
com isso tem 2 modos sequencial e e paralelo o sequencial, a tarefa so deve executar quando a anterior acabar e paralelo todas devem ser iniciadas antes de o ProcessFlow esperar pelo término delas.   

* input / output

faz uma entrada receber dados de um arquivos ex: input ordenar nomes.txt ja o output a saida ser no arquivo ex: output ordenar resultado.txt

* apend 

mesma coisa do output so que vai para o final do arquivo

* workdir

troca onde vai ser executado as tarefas

* pipe

conecta a saida de uma a entrada da outra ex: run pipe listar ordenar contar. a saida de listar vira a entrada de ordenar e a saida de ordenar entrada de contar.

* start 

inicia a tarefa e ja devolve o propt deixando ela executando em segundo plano

* jobs 

lista as tarefas que foram iniciadas em background segundo plano

* wait [jobid]

faz o processflow esperar o job especifico terminar

* workflow

./processflow comandos.pf

ele recebe os comandos de um arquivo por fora

* exit 

faz o programa terminar com ele ou control D do wsl




