/** \file net_udp.c
Ficheiro responsável pela interpretação dos argumentos passados via linha de
comandos. As funções implementadas neste módulo correspondem à interpretação dos
argumentos passados via linha de comandos, e correspondente transferência para a
estrutura de dados transversal aos vários módulos do projecto.
*/

#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "common.h"
#include "net_udp.h"

/**
Função de leitura do endereço e do porto e criação da estrutura de destino.
Esta função recebe os parâmetros conseguidos via linha de comandos e determina
qual a estrutura de destino (relativa ao servidor UDP).

A função getaddrinfo() é utilizada em vez da função gethostbyname(), por uma
questão de preferência pessoal e também pelo facto da função getaddrinfo() estar
especificada como thread-safe (a função gethostbyname() só é thread-safe em
algumas implementações). Os detalhes da função getaddrinfo() podem ser
consultados em [RFC 2553](https://www.ietf.org/rfc/rfc2553.txt).

Na estrutura comum é alocada memória para a struct sockaddr de destino.
*/
void getsockaddr(char*node,char*service,struct startup_data *startup_data){
	int i;
	struct addrinfo *address;
	struct addrinfo hint;
	
	/* Especificação de alguns dos parâmetros através da hint: */
	hint.ai_family = 0; /* Não especificar a família deixa o sistema aberto
		a IPv6, se tal for possível */
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_protocol = 0; /* Podia repetir-se aqui o protocolo, redundante*/
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
	memcpy(&((*startup_data).dest_size),
		&(address[0].ai_addrlen),sizeof(socklen_t));
	
	(*startup_data).destination =
		malloc((*startup_data).dest_size);
	
	memcpy((*startup_data).destination,address[0].ai_addr,
		(*startup_data).dest_size);
	
	(*startup_data).family = address[0].ai_family;
	
	if ((*startup_data).family == AF_UNSPEC){
		(*startup_data).family = AF_INET6;
	}
	
	/* Libertação da lista com os endereços: */
	freeaddrinfo(address);
}
