# Este Makefile assume que o arquivo VetorDePonteiros.h esta 
# no diretorio atual

sc = server_concorrente.c
cl = client.c
TARGETS = client servidor server server_concorrente
CFLAGS = -g -O2 -c
CC = gcc

compile:
	$(CC) $(sc) -o server 
	$(CC) $(cl) -o client   
	java -jar BuilderGen.jar build
	echo Compilado!

run:
	./server -f 8080	

delete:
	@rm -f $(TARGETS)
	@echo Arquivos executáveis removidos!
