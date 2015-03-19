/** \file net_tcp.c
Ficheiro responsável pela implementação das funções de rede associadas ao
protocolo TCP.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "common.h"
#include "net_common.h"
#include "net_tcp.h"

#define RCI_BACKLOG 16

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
	/* A socket de chegada é sempre AF_INET6, que suporta tanto IPv4 como
	IPv6 */
	i = socket(AF_INET6,SOCK_STREAM,0);
	
	if(i<0){
		perror("ddt");
		exit(0);
	}
	
	
	/* Especificação do endereço para o bind() : */
	size = sizeof(struct sockaddr_in6);
	address.sin6_family = AF_INET6;
	
	// /!\ CENAS - atenção à endianness do porto!
	sscanf((*transversal_data).startup_data.ringport,"%d", (int*)
		&(address.sin6_port));
	address.sin6_flowinfo = 0;
	address.sin6_scope_id = 0;
	address.sin6_addr = in6addr_any;
	
	/* Bind da socket aos endereços a máquina. */
	if(bind(i,(struct sockaddr*)&address,size)<0){
		perror("ddt");
		exit(0);
	}
	
	/* Listen: */
	if(listen(i,RCI_BACKLOG)<0){
		perror("ddt");
		exit(0);
	}
	
	/* Armazenamento do index da socket na estrutura transversal: */
	(*transversal_data).t = i;
	
	return;
}

/** Função de abertura de uma sessão TCP com outro nó.

Esta função cria uma socket e liga-a directamente ao nó, retornando então essa
mesma socket.
*/
int connect_tcp(char* node, char* service){
	int i;
	int family = 0;
	
	struct sockaddr* address = NULL;
	socklen_t addr_length;
	
	getsockaddr(node,service,&family,&addr_length,&address,1);
	
	/* Abertura da socket: */
	i = socket(family,SOCK_STREAM,0);
	
	if(i<0){
		perror("ddt");
		exit(0);
	}
	
	if(connect(i,address,addr_length)<0){
		perror("ddt");
		exit(0);
	}
	
	return i;
}
