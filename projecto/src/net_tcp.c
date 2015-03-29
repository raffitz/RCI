/** \file net_tcp.c
Ficheiro responsável pela implementação das funções de rede associadas ao
protocolo TCP.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
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
	int family;
	uint16_t port;

	struct sockaddr* address;

	struct sockaddr_in6 address6;
	struct sockaddr_in address4;
	socklen_t size;


	family = transversal_data->startup_data.family;
	
	/* Abertura da socket: */
	i = socket(family,SOCK_STREAM,0);

	if(i<0){
		perror("ddt:socket");
		exit(0);
	}


	sscanf((*transversal_data).startup_data.ringport,"%hu",&port);

	/* Especificação do endereço para o bind() : */
	if(family == AF_INET){
		size = sizeof(struct sockaddr_in);
		address = (struct sockaddr*) &address4;
		address4.sin_family = AF_INET;
		address4.sin_port = htons(port);
		address4.sin_addr.s_addr = INADDR_ANY;
	}else{
		size = sizeof(struct sockaddr_in6);
		address = (struct sockaddr*) &address6;
		address6.sin6_family = AF_INET6;
		address6.sin6_port = htons(port);
		address6.sin6_flowinfo = 0;
		address6.sin6_scope_id = 0;
		address6.sin6_addr = in6addr_any;
	}
	/* Bind da socket aos endereços a máquina. */
	if(bind(i,(struct sockaddr*)address,size)<0){
		perror("ddt:bind");
		exit(0);
	}

	/* Listen: */
	if(listen(i,RCI_BACKLOG)<0){
		perror("ddt:listen");
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
		perror("ddt:socket");
		exit(0);
	}

	if(connect(i,address,addr_length)<0){
		perror("ddt:connect");
		exit(0);
	}

	return i;
}

/** Função que lê dum file descriptor (socket) uma mensagem. */
int read_message_tcp(char* dest,int destsize, int fd){
	int nread;
	int num_char = 0;
	char readbyte;
	
	char counter = 0;
	
	fd_set to_fds;

	struct timeval timeout;
	
	/* Lê um char de cada vez para verificar quando encontra o '\n': */
	while(1){
		
		
		

	
		for(counter=0;counter<5;counter++){
			FD_ZERO(&to_fds);
			FD_SET(fd,&to_fds);
		
			timeout.tv_sec = 3;
			timeout.tv_usec = 0;
	
			if(select(fd+1,&to_fds,NULL,NULL,&timeout)<1) continue;
	
			if(FD_ISSET(fd,&to_fds)){
				nread=read(fd, &readbyte, 1);
				break;
			}
		/*printf("Timeout elapsed. No contact from server.\n");*/
			nread = -1;
		}
		
		if(nread==-1){
			printf("Erro ao ler do file descriptor\n");
			return -1;
		}else if((nread==0) || (readbyte=='\n')){
			dest[num_char]='\0';
			break;
		}else{
			dest[num_char] = readbyte;
		}
		num_char++;
		if(num_char >= destsize){
			num_char--;
			dest[num_char] = '\0';
			break;
		}
	}
	if (strlen(dest)<=0) return -1;
	return 0;
}
