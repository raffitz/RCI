/** \file net_tcp.c
Ficheiro responsável pela implementação das funções de rede associadas ao
protocolo TCP.
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "common.h"
#include "net_tcp.h"

/** Função de abertura do servidor TCP.

Esta função cria a socket para recepção de mensagens via TCP.
Esta função consiste de socket(), bind() e listen() exclusivamente, o accept
deve ser feito a posteriori, de maneira a não bloquear a execução do programa.
*/
void createserver_tcp(struct transversal_data *transversal_data){
	
	int i;
	
	struct sockaddr_in6 address;
	socklen_t size;
	
	/* Abertura da socket: */
	i = socket(AF_INET6,SOCK_STREAM,0);
	
	if(i<0){
		perror("ddt");
		exit(0);
	}
	
	
	/* Especificação do endereço para o bind() : */
	size = sizeof(sockaddr_in6);
	address.sin6_family = AF_INET6;
	
	/* --/!\-- */
	
	
}
