/** \file net_udp.c
Ficheiro responsável pela implementação das funções de rede associadas ao
protocolo UDP.
*/

#include <stdlib.h>
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
