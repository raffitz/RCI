/** \file main.c
Ficheiro principal do projecto. Este ficheiro serve como chave de abóbada
para os vários módulos desenvolvidos e responsáveis pelas diversas
funcionalidades do projecto.
*/


#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "options.h"
#include "net_udp.h"
#include "net_tcp.h"
#include "interface.h"

/** Função principal do programa ddt. Esta função serve-se dos módulos criados
nos restantes ficheiros-fonte para implementar as funcionalidades necessárias ao
projecto, como especificadas no enunciado.
*/
int main(int argc, char**argv){
	
	struct transversal_data transversal_data;
	struct args_parse_options args_parse_options;
	int ex=0;
	
	/* Inicialização dos valores transversais: */
	transversal_data.u = -1;
	transversal_data.t = -1;
	
	transversal_data.peer_pred.id = -1;
	transversal_data.peer_pred.node[0] = 0;
	transversal_data.peer_pred.service[0] = 0;
	transversal_data.peer_pred.socket = -1;
	
	transversal_data.peer_succ.id = -1;
	transversal_data.peer_succ.node[0] = 0;
	transversal_data.peer_succ.service[0] = 0;
	transversal_data.peer_succ.socket = -1;
	
	transversal_data.reg = -1;
	transversal_data.ring = -1;
	transversal_data.id = -1;
	
	/* Leitura das opções: */
	args_parse_options.startup_data = &(transversal_data.startup_data);
	args_parse_options.argc = &argc;
	args_parse_options.argv = &argv;
	parse_options(&args_parse_options);
	
	
	/* Abertura da socket UDP: */
	createsocket_udp(&transversal_data);
	
	/* Abertura do servidor TCP: */
	createserver_tcp(&transversal_data);
	
	/*Cria a interface do utilizador*/
	ex=interface();	
	
	printf("Testing\n");
	exit(0);
}
