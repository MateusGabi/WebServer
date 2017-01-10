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

typedef const char *string;

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


                string status = "HTTP/1.1 400 File Not Found";

                char resposta[MAX_LINE];

                for (int i = 0; i < 125; ++i)
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

                strcpy(resposta, status);
                strcat(resposta, "\r\n");
                strcat(resposta, "Accept: */*\r\n");
                strcat(resposta, "Date: ");
                strcat(resposta, timebuffer);
                strcat(resposta, "\r\n");
                strcat(resposta, "Connection: close\r\n");
                strcat(resposta, "Content-Type: text/html\r\n");
                strcat(resposta, "Content-Length: ");
                strcat(resposta, "20");
                strcat(resposta, "\r\n");
                strcat(resposta, "Server: FACOM-RC-2016/2.0\r\n");
                strcat(resposta, "\r\n");
                strcat(resposta, "<h1>/ IndexOf </h1>");                
                resposta[MAX_LINE] = '\0';

                
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

string carregarArquivo()
{
    string status = "HTTP/1.1 400 File Not Found"; 
    return status;
} // fileExists()

int verificaPorta(int porta)
{
    
    return porta;   
    
} // verificaPorta()

void mostra_erro()
{
    printf("Um erro ocorreu!\n");
    exit(1);
}