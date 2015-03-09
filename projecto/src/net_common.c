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

Esta função recebe um endereço e 

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
	hint.ai_family = 0; /* Não especificar a família deixa o sistema aberto
		a IPv6, se tal for possível */
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
	}
	memcpy((*sockaddress),address[0].ai_addr,(*size));
	
	(*family) = address[0].ai_family;
	
	if ((*family) == AF_UNSPEC){
		(*family) = AF_INET6;
	}
	
	/* Libertação da lista com os endereços: */
	freeaddrinfo(address);
}
