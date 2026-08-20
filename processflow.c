    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>


    typedef struct task{
        char *nome;
        char *programa;
        char **argumentos;
        int qnt_args;
    }task;

    int tokenizar(){

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
            
        }
        
        return 0;
    }