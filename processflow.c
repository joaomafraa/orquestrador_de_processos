    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>


    typedef struct task{
        char *nome;
        char *programa;
        char **argumentos;
        int qnt_args;
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
        free(verificacao);
        return 0;
    }

    int main(){

        // task *t=(task*)malloc(sizeof(task));
        
        // if(t ==NULL){
        //     printf("Erro ao alocar. ");
        //     return 1;
        // }
        char linha[1024];
        
        while (1)
        {
            printf("processflow> ");
            if(fgets(linha,1024,stdin)==NULL){
                break;
            }
            
            if(processar_comando(linha)){
                printf("TESTE PASSOU");
                break;
            }


        }
        
        return 0;
    }