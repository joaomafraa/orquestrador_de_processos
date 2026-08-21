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


    int main(){

        task *t=(task*)malloc(sizeof(task));
        
        if(t ==NULL){
            printf("Erro ao alocar. ");
            return 1;
        }
        char linha[1024];
        
        while (1)
        {
            printf("processflow> ");
            if(fgets(linha,1024,stdin)==NULL){
                break;
            }
            int qnt_tokens = qnt_token(linha);
            
            printf("teste funcao %d" , qnt_tokens);

            char**tokens = tokenizar(linha,qnt_tokens);
            printf("teste funcao 2 %s %s %s",tokens[0],tokens[1],tokens[2]);
            break;

        }
        
        return 0;
    }