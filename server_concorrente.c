#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define MAX_PENDING 5
#define MAX_LINE 1024
#define SERVER_NAME "FACOM-RC-2016/2.0"

typedef const char *string;

int main(int argc, char *argv[]) {
    
    /* verificações */
    if(argc == 3)
    {
       if(strcmp(argv[1], "-f") == 0)
       {
           printf("Inicializando servidor na porta %s\n", argv[2]);

           int porta = atoi(argv[2]);
           
           use_fork(porta);
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
} // main()

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

    /* loop principal */
    while (1) {

        int connfd = accept(s, (struct sockaddr *)&sin, &len); aceita

        if(connfd < 0)
        {
            error("Falha ao aceitar requisição.");
            exit(1);
        }
        else {
            printf("\n\n------------------ Novo cliente conectado -----------------\n");
        }
        
        pid_t pid;

        if ( (pid = fork()) == 0 ) {

            close(s); /* child closes listening socket */

            /* codigo de execução */
            
            while (len = recv(connfd, buf, sizeof(buf), 0))
            {              
                
                printf("\n\n-------------------- Requisição do cliente ------------------\n\n");
                               
                // fputs(buf, stdout);    
                
                printf("\n\n--------------------- Resposta do Servidor ------------------\n\n");


                /* leitura de arquivo */

                

                /* fim leitura de arquivo */


                string status = "HTTP/1.1 400 File Not Found";

                char resposta[MAX_LINE];

                /* limpa buffer */
                for (int i = 0; i < MAX_LINE; ++i)
                {
                    resposta[i] = 0;
                }

                /* configurando hora */
                time_t rawtime;
                struct tm *info;
                char timebuffer[80];

                time(&rawtime);
                info = localtime(&rawtime);
                strftime(timebuffer, 80, "%a, %d %b %Y %X %Z", info);

                /* fim da config de hora */

                string conteudoHTML = "IndexOf /";    

                strcpy(resposta, status);
                strcat(resposta, "\r\n");
                strcat(resposta, "Accept: */*\r\n");
                strcat(resposta, "Date: ");
                strcat(resposta, timebuffer);
                strcat(resposta, "\r\n");
                strcat(resposta, "Connection: close\r\n");
                strcat(resposta, "Content-Type: text/html\r\n");
                strcat(resposta, "Content-Length: ");
                strcat(resposta, "9");
                strcat(resposta, "\r\n");
                strcat(resposta, "Server: ");
                strcat(resposta, SERVER_NAME);
                strcat(resposta, "\r\n");
                strcat(resposta, "\r\n");
                strcat(resposta, conteudoHTML);

                strcat(resposta, "\0");
                
                fputs(resposta, stdout);            
                
                send(connfd, resposta, strlen(resposta), 0);   
    
            }            
            
            
            /* fim do código em execução */

            close(connfd);
            exit(0);  /* child terminates */
        }
        
        close(connfd);  /*parent closes connected socket*/
    }
    
} // use_fork()

void mostra_erro()
{
    printf("Um erro ocorreu!\n");
    exit(1);
}