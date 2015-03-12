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
Estrutura com a informação referente a um par.
*/
struct peer_data{
	/** Número do nó pelo qual o par é responsável. */
	int id;
	/** Endereço do par. */
	char node[256];
	/** Porto do par. */
	char service[16];
	/** Socket TCP com o par */
	int socket;
}

/**
Estrutura com as referências necessárias aos vários módulos do projecto.
*/
struct transversal_data{
	/** Socket UDP. */
	int u;
	/** Socket TCP [accept]. */
	int t_a;
	/** Estrutura do predecessor. */
	struct peer_data peer_pred;
	/** Estrutura do sucessor. */
	struct peer_data peer_succ;
	/** Anel a ser utilizado. */
	int ring;
	/** Identificador actual do nó. */
	int id;
	/** Registered Node. */
	char reg;
	/** Parâmetros da linha de comandos. */
	struct startup_data startup_data;
	/** IP externo do servidor. */
	char ext_addr[40];
};

#endif
