# Este Makefile assume que o arquivo VetorDePonteiros.h esta 
# no diretorio atual

sc = server_concorrente.c
TARGETS = client server server_concorrente
CFLAGS = -g -O2 -c
CC = gcc

compile:
	$(CC) $(sc) -o server
	@echo Compilado!

run:
	./server -f 8080	

delete:
	@rm -f $(TARGETS)
	@echo Arquivos executáveis removidos!
