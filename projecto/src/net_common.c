/** \file net_common.c
Ficheiro responsável pela implementação das funções relacionadas com rede mas
não associadas a nenhum dos protocolos utilizados.
*/

#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "common.h"
#include "net_common.h"

/**
Função de leitura de endereços e portos e criação de estruturas de destino.

Esta função recebe um endereço e um porto, bem como a família (que é tanto uma
entrada como uma saída), o tamanho da estrutura de saída e um ponteiro para a
estrutura de saída. Caso esse ponteiro seja NULL, a estrutura é alocada.

A função getaddrinfo() é utilizada em vez da função gethostbyname(), por uma
questão de preferência pessoal e também pelo facto da função getaddrinfo() estar
especificada como thread-safe (a função gethostbyname() só é thread-safe em
algumas implementações). Os detalhes da função getaddrinfo() podem ser
consultados em [RFC 2553](https://www.ietf.org/rfc/rfc2553.txt).
*/
void getsockaddr(char *node,char *service,int *family,socklen_t *size,
	struct sockaddr **sockaddress,int protocol)
{
	int i;
	struct addrinfo *address;
	struct addrinfo hint;
	
	/* Especificação de alguns dos parâmetros através da hint: */
	if((*family)==AF_INET){
		hint.ai_family = AF_INET;
	}else if((*family)==AF_INET6){
		hint.ai_family = AF_INET6;
	}else{
		hint.ai_family = 0;
	}
	if(protocol == 0){
		hint.ai_socktype = SOCK_DGRAM; /* UDP */
	}else{
		hint.ai_socktype = SOCK_STREAM; /* TCP */
	}
	
	hint.ai_protocol = 0;
	hint.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;
	hint.ai_addrlen = 0;
	hint.ai_addr = NULL;
	hint.ai_canonname = NULL;
	hint.ai_next = NULL;
	
	
	/* A estrutura de destino é determinada via getaddrinfo() aqui: */
	i = getaddrinfo(node,service,&hint,&address);
	if(i != 0){
		printf("Error getting address info: %s\n",gai_strerror(i));
		exit(0);
	}
	/* Cópia da estrutura relevante e do seu tamanho para a estrutura
	comum */
	memcpy((void*)size,(void*)&(address[0].ai_addrlen),sizeof(socklen_t));
	
	if((*sockaddress)==NULL){
		(*sockaddress) = malloc((*size));
		if(*sockaddress == NULL){
			printf("Out of memory. Terminating.\n");
			exit(0);
		}
	}
	memcpy((*sockaddress),address[0].ai_addr,(*size));
	
	(*family) = address[0].ai_family;
	
	if ((*family) == AF_UNSPEC){
		(*family) = AF_INET6;
	}
	
	/* Libertação da lista com os endereços: */
	freeaddrinfo(address);
}



void getIP(struct sockaddr* me, socklen_t mysize,char*node, char*service)
{
	struct sockaddr_in *aux4;
	struct sockaddr_in6 *aux6;
	uint32_t address_4;
	uint32_t port_nr;
	
	if(me->sa_family == AF_INET){
		aux4 = (struct sockaddr_in*) me;
		if(node!=NULL){
			address_4 = (*aux4).sin_addr.s_addr;
			sprintf(node,"%hhu.%hhu.%hhu.%hhu",
				((char*) &address_4)[0],
				((char*) &address_4)[1],
				((char*) &address_4)[2],
				((char*) &address_4)[3]);
		}
		if(service!=NULL){
			port_nr = ntohl((*aux4).sin_port);
			sprintf(service,"%d",port_nr);
		}
	}else if(me->sa_family == AF_INET6){
		aux6 = (struct sockaddr_in6*) me;
		sprintf(node,"%02hhx%02hhx:"
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
			
		if(service!=NULL){
			port_nr = ntohl((*aux6).sin6_port);
			sprintf(service,"%d",port_nr);
		}
		
	}else{
		printf("Parece-me que anda aqui magia negra...\n");
		exit(0);
	}
}

void write_message(char * buffer, int fd){
	int nleft=strlen(buffer);
	int nwritten;
	if(fd!=-1){
		while(nleft>0){
			nwritten=write(fd,buffer,nleft);
			if(nwritten<=0){
				//error
			}
			nleft-=nwritten;
			buffer+=nwritten;
		}
	}
	return;
}
