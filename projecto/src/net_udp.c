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
	
	getsockaddr(node,service,&((*startup_data).family),
		&((*startup_data).dest_size),&((*startup_data).destination),0);
	
}

/**
Função de abertura da socket UDP com o servidor de arranque.

Esta função recebe como parâmetros a estrutura de dados transversal ao programa
e abre uma socket UDP tendo como destino o servidor de arranque.

Adicionalmente, ao abrir a socket é utilizada a função getsockname() de maneira
a determinar o endereço IP externo da máquina.
*/
void createsocket_udp(struct transversal_data *transversal_data){
	int i;
	
	sa_family_t family;
	
	struct sockaddr *origin;
	socklen_t origin_size;
	
	struct sockaddr_in *aux4;
	struct sockaddr_in6 *aux6;
	
	uint32_t address_4;
	
	family = ((*transversal_data).startup_data).family;
	
	i = socket(family,SOCK_DGRAM,0);
	if (i<0){
		perror("ddt");
		exit(0);
	}
	(*transversal_data).u = i;
	
	if(connect(i,((*transversal_data).startup_data).destination,
		((*transversal_data).startup_data).dest_size) < 0){
		
		perror("ddt");
	}
	
	
	
	if(family == AF_INET){ /* IPv4: */
		origin_size = sizeof(struct sockaddr_in);
		origin = malloc(origin_size);
		if(getsockname(i,origin,&origin_size)<0){
			perror("ddt");
			exit(0);
		}
		aux4 = (struct sockaddr_in *)origin;
		address_4 = (*aux4).sin_addr.s_addr;
		sprintf((*transversal_data).ext_addr,"%hhu.%hhu.%hhu.%hhu",
			((char*) &address_4)[0],((char*) &address_4)[1],
			((char*) &address_4)[2],((char*) &address_4)[3]);
		
	}else{ /* IPv6: */
		origin_size = sizeof(struct sockaddr_in6);
		origin = malloc(origin_size);
		if(getsockname(i,origin,&origin_size)<0){
			perror("ddt");
			exit(0);
		}
		aux6 = (struct sockaddr_in6 *)origin;
		sprintf((*transversal_data).ext_addr,
"%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx:%02hhx%02hhx",
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
		
	}
	
	printf("External Address: %s\n",(*transversal_data).ext_addr);
	
	free(origin);
}
