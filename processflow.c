    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h> 
    #include <sys/wait.h>
    #include <fcntl.h>

    typedef struct task{
        char *nome;
        char *programa;
        char **argumentos;
        int qnt_args;

        char *input;
        char *output;
        int append;
    }task;

    int qnt_token(char *linha){
        char* aux = (char* )malloc(strlen(linha) +1);
        if(aux == NULL) return -1; 
        strcpy(aux,linha);
        int cont =0;
        char *token = strtok(aux," \t\n");
        while(token!=NULL){
            token = strtok(NULL," \t\n");
            cont++;
        }
        free(aux);
        return cont;
    }
    char** tokenizar(char*linha,int qnt){
        char **tokens = (char **)malloc((qnt +1 )* sizeof(*tokens));
        if(tokens == NULL) return NULL;
        int cont=0;
        char *token = strtok(linha," \n\t");
        while(token!=NULL){
            tokens[cont] = token;
            token = strtok(NULL," \t\n");
            cont++;
        }
        tokens[cont] =NULL;
        return tokens;
    }

    int processar_comando(char *linha){
        int qtd =qnt_token(linha);
        if(qtd== 0){
            return 0;
        }
        char** verificacao = tokenizar(linha,qtd);
        if(verificacao == NULL) return 0;
        
        
        if(strcmp(verificacao[0], "exit") == 0){
            free(verificacao);
            return 1;
        }
        if(strcmp(verificacao[0], "task") == 0){
            free(verificacao);
            
            return 2;
        }
        if(strcmp(verificacao[0], "run") == 0){
            free(verificacao);
            return 3;
        }
        if(strcmp(verificacao[0], "input") == 0){
            free(verificacao);
            return 4;
        }
        if(strcmp(verificacao[0], "output") == 0){
            free(verificacao);
            return 4;
        }if(strcmp(verificacao[0], "append") == 0){
            free(verificacao);
            return 4;
        }
        free(verificacao);
        return 0;
    }

task *cadastrar_task(char **lista_tarefas, int qnt_tokens, int *qnt_tarefas, task *tarefas){

    if(qnt_tokens < 3){
        printf("Erro: parametros insuficientes\n");
        return tarefas;
    }

    task *aux = (task*)realloc(tarefas,(*qnt_tarefas + 1) * sizeof(task));
    if(aux == NULL){
        printf("Erro de alocacao\n");
        return tarefas;
    }

    tarefas = aux;

    task *nova = &tarefas[*qnt_tarefas];
    nova->input = NULL;
    nova->output = NULL;
    nova->append = 0;

    nova->nome = malloc(strlen(lista_tarefas[1]) + 1);
    strcpy(nova->nome, lista_tarefas[1]);

    nova->programa = malloc(strlen(lista_tarefas[2]) + 1);
    strcpy(nova->programa, lista_tarefas[2]);
    
    nova->qnt_args = qnt_tokens - 2;

    nova->argumentos =malloc((nova->qnt_args + 1) * sizeof(char *));

    int j = 0;
    for(int i = 2; i <qnt_tokens; i++){
        nova->argumentos[j] =malloc(strlen(lista_tarefas[i]) + 1);

        strcpy(nova->argumentos[j], lista_tarefas[i]);
        j++;
    }
    nova->argumentos[nova->qnt_args] = NULL;
    (*qnt_tarefas)++;
    return tarefas;
}
task *buscar_task(char *nome,task *tarefas,int qnt_tarefas){
    for(int i=0;i<qnt_tarefas;i++){
        if(strcmp(tarefas[i].nome,nome) == 0){
            return &tarefas[i];
        }
    }
    return NULL;
}
pid_t executar_task(task *tarefas){
    pid_t pid=fork();
    if(pid < 0){
        perror("Erro ao executar programa");//funcao perror so entra caso de erro e imprime essa mensagem + o motivo
        return -1;
    } 
    if(pid ==0){//entrei no processo filho
        if(tarefas->output != NULL){
            int fd;
            if(tarefas->append == 1){
            fd =open(tarefas->output,O_WRONLY | O_CREAT | O_APPEND,0644);//nome do arq|somente escrita|se nao existir cria||escreva no final|permissoes
            }
        else{
            fd = open(tarefas->output,O_WRONLY | O_CREAT | O_TRUNC,0644);
        }
            if(fd ==-1){
                perror("open");
                _exit(1);
            }
            dup2(fd, STDOUT_FILENO);//ele liga a sainda padrao que é 1/stdout... no arquivo
            close(fd);
        }
        if(tarefas->input != NULL){
            int fd = open(tarefas->input,O_RDONLY);
            if(fd ==-1){
                perror("open");
                _exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        //entrou no if processo filho é substituido pela chamda do programa registado 
        //na task
        execv(tarefas->programa,tarefas->argumentos);

        //se falhar vai vir pr k
        perror("execv");
        _exit(1);
    }
    return pid;
    
}
void run_task(char **tokens, int qnt_tokens, task *tarefas, int qnt_tarefas){
    if(qnt_tokens < 2){
        printf("Erro: parametros insuficientes para run\n");
        return;
    }
    task *t =buscar_task(tokens[1], tarefas, qnt_tarefas);
    if(t == NULL){
        printf("Erro: tarefa %s nao existe\n", tokens[1]);
        return;
    }
    pid_t pid = executar_task(t);
    if(pid > 0){
        int status;
        waitpid(pid, &status, 0);
        if(WIFEXITED(status)){
            int codigo = WEXITSTATUS(status);
            if(codigo!=0){
                printf("Tarefa %s terminou com codigo %d\n",t->nome,codigo);
            }
        }
    }
}
void run_sequential(char **tokens, int qnt_tokens,task *tarefas, int qnt_tarefas){
    
    if(qnt_tokens <3){
        printf("Erro: parametros insuficientes para run\n");
        return;
    }
    for(int i = 2; i < qnt_tokens; i++){
        task *t = buscar_task(tokens[i],tarefas,qnt_tarefas);
        if(t == NULL){
            printf("Erro: tarefa %s nao existe\n", tokens[i]);
            continue;
        }
        pid_t pid=executar_task(t);//criou o filho 
        if(pid > 0){//criou com sucesso entra aq
            int status;
            waitpid(pid, &status, 0);//manda o pai esperar e agora vai pegar o status do codigo dps de esperar executar 
            if(WIFEXITED(status)){//teminou o processo filho entrou aq para analisar o estado de saida
                int codigo = WEXITSTATUS(status); //pega a saida real do codigo
                if(codigo !=0){//deu ruim e entra aq
                    printf("Tarefa %s deu erro e terminou com codigo %d: \n",t->nome,codigo);
                }
            }
        }
    }
}

void run_parallel(char **tokens, int qnt_tokens,task *tarefas, int qnt_tarefas){
    if(qnt_tokens <3){
        printf("Erro: parametros insuficientes para run\n");
        return;
    }

    pid_t* pids=(pid_t*)malloc((qnt_tokens - 2) * sizeof(pid_t)); //armazenar os pid que vao ser criados
    if(pids == NULL){
        printf("Erro alocacao");
        return;
    }
    int qnt_pids = 0;
    for(int i=2; i<qnt_tokens; i++){
        task *t = buscar_task(tokens[i],tarefas,qnt_tarefas); 
        if(t == NULL){
            printf("Erro: tarefa %s nao existe\n", tokens[i]);
            continue;
        }
        
        pid_t pid = executar_task(t);
        if(pid>0){
            pids[qnt_pids] =pid;
            qnt_pids ++;
        }
    }

    for(int i=0;i<qnt_pids;i++){//outro for para deixar o processo pai esperando os pids acabar
        int status;
        waitpid(pids[i],&status,0);
            if(WIFEXITED(status)){
                int codigo=WEXITSTATUS(status);
                if(codigo !=0){
                    printf("Processo %d deu erro e terminou com codigo %d: \n",pids[i],codigo);
                }
            }
        }
        free(pids);
    }
void configurar_red(char **tokens, int qnt_tokens,task *tarefas, int qnt_tarefas){
    if(qnt_tokens<3){
        printf("Erro: parametros insuficientes\n");
        return;
    }
    task*t=buscar_task(tokens[1],tarefas,qnt_tarefas);
    if(t == NULL){
        printf("Erro: tarefa %s nao existe\n", tokens[1]);
        return;
    }
    if(strcmp(tokens[0], "input") == 0){
        t->input = malloc(strlen(tokens[2])+1);
        strcpy(t->input, tokens[2]);
    }
    if(strcmp(tokens[0], "output") == 0){
        t->output = malloc(strlen(tokens[2])+1);
        strcpy(t->output, tokens[2]);
        t->append=0;
    }
    if(strcmp(tokens[0], "append") == 0){
    t->output = malloc(strlen(tokens[2]) + 1);
    strcpy(t->output, tokens[2]);
    t->append = 1;
}
    

}
int main(){

    task *tarefas = NULL;
    int qnt_tarefas = 0;

    char linha[1024];

    while(1){

        printf("processflow> ");

        if(fgets(linha, 1024, stdin) == NULL){
            break;
        }
        char copia[1024];
        strcpy(copia, linha);

        int comando = processar_comando(copia);

        if(comando == 1){
            break;
        }
        if(comando == 2){
            int tokens = qnt_token(linha);
            char **lista_separada = tokenizar(linha, tokens);
            tarefas = cadastrar_task(lista_separada,tokens,&qnt_tarefas,tarefas);


            free(lista_separada);
        
        }
        if(comando == 3){
            int tokens = qnt_token(linha);
            char **lista_separada = tokenizar(linha, tokens);
            if(lista_separada == NULL){
                continue;
            }
            if(tokens < 2){
                printf("Erro: parametros insuficientes para run\n");

                free(lista_separada);

                continue;
            }
           
            if(strcmp(lista_separada[1], "sequential") == 0){
                run_sequential(lista_separada,tokens,tarefas,qnt_tarefas);
            }
            else if(strcmp(lista_separada[1], "parallel") == 0){
                run_parallel(lista_separada,tokens,tarefas,qnt_tarefas);
            }else{
                run_task(lista_separada,tokens,tarefas,qnt_tarefas);
            }
            free(lista_separada);
            }
        if(comando == 4){
            int tokens = qnt_token(linha);
            char **lista_separada = tokenizar(linha, tokens);
            if(lista_separada == NULL){
                continue;
            }
            configurar_red(lista_separada,tokens,tarefas,qnt_tarefas);
            free(lista_separada);
        }
    }

    return 0;
}