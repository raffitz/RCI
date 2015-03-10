/** \file common.h
Cabeçalho com declarações das estruturas de dados transversais aos vários
módulos do projecto.
*/

#ifndef RCI_COMMON
#define RCI_COMMON

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

/**
Estrutura com os dados a ser recolhidos pela linha de comandos.
*/
struct startup_data{
	/** Porto do servidor TCP. */
	char ringport[16];
	/** Família da socket */
	int family;
	/** Estrutura de destino para as mensagens UDP. */
	struct sockaddr *destination;
	/** Tamanho da estrutura de destino para as mensagens UDP. */
	socklen_t dest_size;
};

/**
Estrutura com as referências necessárias aos vários módulos do projecto.
*/
struct transversal_data{
	/** Socket UDP. */
	int u;
	/** Socket TCP. */
	int t;
	/** Anel a ser utilizado. */
	int ring;
	/** Parâmetros da linha de comandos. */
	struct startup_data startup_data;
	/** IP externo do servidor. */
	char ext_addr[40];
};

#endif
