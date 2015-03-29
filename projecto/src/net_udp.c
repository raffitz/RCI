/** \file net_udp.c
Ficheiro responsável pela implementação das funções de rede associadas ao
protocolo UDP.
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
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
	/* (*startup_data).family = AF_INET; */

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


#ifdef RCIDEBUG2

	char buf[256];
	
	char counter = 0;
	
	fd_set to_fds;
	
	struct timeval timeout;

	
	for(counter=0;counter<5;counter++){
		FD_ZERO(&to_fds);
		FD_SET(i,&to_fds);
		
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
		
		sendto(i,"BQRY 5",6,0,((*transversal_data).startup_data).
			destination,((*transversal_data).startup_data).
			dest_size);
	
		if(select(i+1,&to_fds,NULL,NULL,&timeout)<1) continue;
	
		if(FD_ISSET(i,&to_fds)){
			buf[recvfrom(i,buf,256,0,NULL,NULL)] = 0;
			printf("RCIDEBUG1: UDP: Sent BQRY 5 to server."
				"\nServer:%s\n",buf);
			break;
		}
		printf("Timeout elapsed. No contact from server.\n");
	}
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

	sockaddr.sa_family = AF_UNSPEC;/* Para fazer reset do peer address */

	if(connect((*transversal_data).u,
	(*transversal_data).startup_data.destination,
	(*transversal_data).startup_data.dest_size)<0){
		perror("ddt:connect");
		exit(0);
	}

	/* GetSockName: */
	if(getsockname((*transversal_data).u,(struct sockaddr*)&me,&mysize)<0){
		perror("ddt:getsockname");
		exit(0);
	}


	getIP((struct sockaddr*) &me,mysize,(*transversal_data).ext_addr,NULL);

#ifdef RCIDEBUG1
	printf("RCIDEBUG1: My IP: %s\n",(*transversal_data).ext_addr);
#endif
	/* Reset do porto UDP: */
	if(connect((*transversal_data).u,&sockaddr,length)<0){
		perror("ddt:connect");
		exit(0);
	}

}
