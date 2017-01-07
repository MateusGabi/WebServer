#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_PENDING 5
#define MAX_LINE 256

int main(int argc, char *argv[]) {
    
    /* servidor -f PORTA */
    verificadorDeArgumentos(argc, argv);  

    
} // main()

void verificadorDeArgumentos(int argc, char *argv[])
{
     if(argc == 3 )
     {
         if(strcmp(argv[1], "-f") == 0)
         {
             printf("Inicializando servidor na porta %s\n", argv[2]);
             
             int porta = atoi(argv[2]);
             
             if(verificaPorta(porta))
             {
                 use_fork(porta);
             }
             else
             {
                 printf("Erro na porta\n");
             }
         }
         else {
             mostra_erro();
         }
     }
     else if(argc == 4)
     {
         if(strcmp(argv[1], "-t") == 0)
         {
             printf("Não implementado.");
         }
         else {
             mostra_erro();
         }
     }
     else
     {
        printf("Há dois métodos para inicialização do servidor:\n");
        printf("./servidor -f PORTA\n");
        printf("./servidor -t N PORTA\n"); 
        exit(1);    
     }
    
} // verificadorDeArgumentos()

void use_fork(int porta)
{
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int len, s, new_s;

    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(porta);

    /* setup passive open */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("simplex-talk: socket");
        exit(1);
    }

    if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0)
    {
        perror("simplex-talk: bind");
        exit(1);
    }

    listen(s, MAX_PENDING);

    /* wait for connection, then receive and print text */
    while (1) {

        int connfd = accept(s, (struct sockaddr *)&sin, &len); /* blocking call */

        if(connfd < 0)
        {
            error("Falha ao aceitar requisição.");
            exit(1);
        }
        else {
            printf("\n\n------------------ Novo cliente conectado -----------------\n\n");
        }
        
        pid_t pid;

        if ( (pid = fork()) == 0 ) {

            close(s); /* child closes listening socket */

            /* codigo de execução */
            
            while (len = recv(connfd, buf, sizeof(buf), 0))
            {              
                
                printf("\n\n-------------------- Requisição do cliente ------------------\n\n");
                               
                fputs(buf, stdout);    
                
                printf("\n\n--------------------- Resposta do Servidor ------------------\n\n");
                               
                char resposta[MAX_LINE];
                
                resposta[0] = "";
    
                strcat(resposta, "HTTP/1.1 404 File Not Found\r\n");
                strcat(resposta, "Server: FACOM-RC-2016/2.0\r\n");
                strcat(resposta, "Content-type: text/plain\r\n\r\n");
                strcat(resposta, "Could not find the specified URL\r\n");
                
                resposta[MAX_LINE] = '\0';
                
    
                // char resposta = carregarResposta();
                
                fputs(resposta, stdout);            
                
                send(connfd, resposta, sizeof(resposta), 0);       
            }            
            
            
            /* fim do código em execução */

            close(connfd);
            exit(0);  /* child terminates */
        }
        
        close(connfd);  /*parent closes connected socket*/
    }
    
} // use_fork()

int verificaPorta(int porta)
{
    
    return porta;   
    
} // verificaPorta()

void mostra_erro()
{
    printf("Um erro ocorreu!\n");
    exit(1);
}