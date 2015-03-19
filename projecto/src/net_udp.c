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
	
	family = (*transversal_data).startup_data.family;
	
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

/**
Função de determinação do IP externo.

Esta função recebe como parâmetros a estrutura de dados transversal ao programa
e serve-se da socket UDP e do servidor de arranque de maneira a determinar o IP
da interface a ser, de facto, utilizada.

*/
void probe_my_IP(struct transversal_data *transversal_data){
	struct sockaddr sockaddr;
	socklen_t length = sizeof(struct sockaddr);
	
	struct sockaddr_storage me;
	socklen_t mysize = sizeof(struct sockaddr_storage);
	
	struct sockaddr_in *aux4;
	struct sockaddr_in6 *aux6;
	uint32_t address_4;
	
	
	sockaddr.sa_family = AF_UNSPEC;/* Para fazer reset do peer address */
	
	if(connect((*transversal_data).u,
	(*transversal_data).startup_data.destination,
	(*transversal_data).startup_data.dest_size)<0){
		perror("ddt");
		exit(0);
	}
	
	/* GetSockName: */
	if(getsockname((*transversal_data).u,(struct sockaddr*)&me,&mysize)<0){
		perror("ddt");
		exit(0);
	}
	
	if(me.ss_family == AF_INET){
		aux4 = (struct sockaddr_in*) &me;
		address_4 = (*aux4).sin_addr.s_addr;
		sprintf((*transversal_data).ext_addr,"%hhu.%hhu.%hhu.%hhu",
			((char*) &address_4)[0],((char*) &address_4)[1],
			((char*) &address_4)[2],((char*) &address_4)[3]);
	}else if(me.ss_family == AF_INET6){
		aux6 = (struct sockaddr_in6*) &me;
		sprintf((*transversal_data).ext_addr,"%02hhx%02hhx:"
			"%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:"
			"%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx",
			(*aux6).sin6_addr.s6_addr[0],
			(*aux6).sin6_addr.s6_addr[1],
			(*aux6).sin6_addr.s6_addr[2],
			(*aux6).sin6_addr.s6_addr[3],
			(*aux6).sin6_addr.s6_addr[4],
			(*aux6).sin6_addr.s6_addr[5],
			(*aux6).sin6_addr.s6_addr[6],
			(*aux6).sin6_addr.s6_addr[7],
			(*aux6).sin6_addr.s6_addr[8],
			(*aux6).sin6_addr.s6_addr[9],
			(*aux6).sin6_addr.s6_addr[10],
			(*aux6).sin6_addr.s6_addr[11],
			(*aux6).sin6_addr.s6_addr[12],
			(*aux6).sin6_addr.s6_addr[13],
			(*aux6).sin6_addr.s6_addr[14],
			(*aux6).sin6_addr.s6_addr[15]);
		
	}else{
		printf("Parece-me que anda aqui magia negra...\n");
		exit(0);
	}
	
	if(connect((*transversal_data).u,&sockaddr,length)<0){
		perror("ddt");
		exit(0);
	}
}
