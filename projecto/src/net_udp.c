/** \file net_udp.c
Ficheiro responsável pela implementação das funções de rede associadas ao
protocolo UDP.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "common.h"
#include "net_common.h"
#include "net_udp.h"

/**
Função de determinação do destino do protocolo UDP.

Esta função recebe o endereço e porto do servidor de arranque e coloca na
estrutura comum a estrutura correspondente a esse endereço.

Na estrutura comum é alocada memória para a struct sockaddr de destino.
*/
void getudpdest(char*node,char*service,struct startup_data *startup_data){
	
	(*startup_data).destination = NULL;
	
	/* Uma vez que o servidor disponibilizado pela docência não suporta
	IPv6, é necessário forçar essa socket UDP para ser IPv4*/
	(*startup_data).family = AF_INET;
	
	getsockaddr(node,service,&((*startup_data).family),
		&((*startup_data).dest_size),&((*startup_data).destination),0);
	
}

/**
Função de abertura da socket UDP com o servidor de arranque.

Esta função recebe como parâmetros a estrutura de dados transversal ao programa
e abre uma socket UDP tendo como destino o servidor de arranque.

*/
void createsocket_udp(struct transversal_data *transversal_data){
	int i;
	
	sa_family_t family;
	
	family = ((*transversal_data).startup_data).family;
	
	i = socket(family,SOCK_DGRAM,0);
	if (i<0){
		perror("ddt");
		exit(0);
	}
	(*transversal_data).u = i;
	
	
#ifdef RCIDEBUG1

	char buf[256];

	sendto(i,"BQRY 5",6,0,((*transversal_data).startup_data).destination,
		((*transversal_data).startup_data).dest_size);
	buf[recvfrom(i,buf,256,0,NULL,NULL)] = 0;
	printf("RCIDEBUG1: UDP Test. Sent BQRY 5 to server.\nServer:%s\n",buf);
#endif
}
