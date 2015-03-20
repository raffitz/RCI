/** \file main.c
Ficheiro principal do projecto. Este ficheiro serve como chave de abóbada
para os vários módulos desenvolvidos e responsáveis pelas diversas
funcionalidades do projecto.
*/

#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "options.h"
#include "net_udp.h"
#include "net_tcp.h"
#include "interface.h"
#include "message_handler.h"

#define STDIN 0

/** Função principal do programa ddt. Esta função serve-se dos módulos criados
nos restantes ficheiros-fonte para implementar as funcionalidades necessárias ao
projecto, como especificadas no enunciado.
*/
int main(int argc, char**argv){

	struct transversal_data transversal_data;
	int counter, max_fd;
	fd_set fds;

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

	transversal_data.ring = -1;
	transversal_data.id = -1;
	transversal_data.serv_arranq = 0;

	/* Leitura das opções: */
	parse_options(&argc, &argv, &transversal_data);


	/* Abertura da socket UDP: */
	createsocket_udp(&transversal_data);
	
	/* Determinação do IP externo (via UDP): */
	probe_my_IP(&transversal_data);

	/* Abertura do servidor TCP: */
	createserver_tcp(&transversal_data);

	/*Cria a interface do utilizador*/
	print_ui();

	FD_ZERO(&fds);
	FD_SET(STDIN, &fds);
	max_fd=STDIN;

	printf(">");

	while(1){

		counter = select(max_fd+1, &fds, (fd_set*)NULL, (fd_set*)NULL, (struct timeval *)NULL);
		if(counter <= 0){
			exit(1);
		}

		if(FD_ISSET(STDIN, &fds)){
			if (interface(&transversal_data)){
				break;
			}else{
				printf(">");
			}
		}
		if(FD_ISSET(transversal_data.t ,&fds)){
			//alguem esta-se a liagr a nos (select)
		}
		if(transversal_data.peer_pred.socket>=0){
			if(FD_ISSET(transversal_data.peer_pred.socket ,&fds)){
				//predecessor mandou uma mensagem
			}
		}
		if(transversal_data.peer_succ.socket>=0){
			if(FD_ISSET(transversal_data.peer_succ.socket ,&fds)){
				//sucessor mandou uma mensagem
			}
		}
	}
#ifdef RCIDEBUG1
	printf("Testing\n");
#endif
	exit(0);
}
