/** \file main.c
Ficheiro principal do projecto. Este ficheiro serve como chave de abóbada
para os vários módulos desenvolvidos e responsáveis pelas diversas
funcionalidades do projecto.
*/

#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "common.h"
#include "options.h"
#include "net_common.h"
#include "net_udp.h"
#include "net_tcp.h"
#include "interface.h"
#include "message_handler.h"

#define RCI_READSIZE 256

#define STDIN 0

/** Função principal do programa ddt. Esta função serve-se dos módulos criados
nos restantes ficheiros-fonte para implementar as funcionalidades necessárias ao
projecto, como especificadas no enunciado.
*/
int main(int argc, char**argv){

	struct transversal_data transversal_data;
	int counter, max_fd;
	char buffer[RCI_READSIZE];

	/* Variáveis para o accept: */
	int new_fd;

	/* Conjunto de file descriptors: */
	fd_set fdset;

	/* Variável para o Signal Handler desactivado: */
	void (*old_handler)(int);

	/* Desactivação do signal handler: */
	if((old_handler=signal(SIGPIPE,SIG_IGN))==SIG_ERR) exit(0);

	/* Inicialização dos valores transversais: */
	reset_td(&transversal_data);

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
	printf(">");
	fflush(stdout);


	while(1){
		/* Inicializa lista de fd's .*/
		FD_ZERO(&fdset);
		/* Para cada file descriptor que vamos ouvir, adiciona-o ao
		select se estiver correntemente válido: */
		FD_SET(STDIN, &fdset);
		max_fd=STDIN;
		if(transversal_data.t >= 0){
#ifdef RCIDEBUG2
			printf("Inserting Accept socket into select\n");
#endif
			FD_SET(transversal_data.t, &fdset);
			if(transversal_data.t>max_fd) max_fd=transversal_data.t;
		}
		if(transversal_data.peer_pred.socket >= 0){
#ifdef RCIDEBUG2
			printf("Inserting Pred socket into select\n");
#endif
			FD_SET(transversal_data.peer_pred.socket, &fdset);
			if(transversal_data.peer_pred.socket>max_fd)
				max_fd=transversal_data.peer_pred.socket;
		}
		if(transversal_data.peer_succ.socket >= 0){
#ifdef RCIDEBUG2
			printf("Inserting Succ socket into select\n");
#endif
			FD_SET(transversal_data.peer_succ.socket, &fdset);
			if(transversal_data.peer_succ.socket>max_fd)
				max_fd=transversal_data.peer_succ.socket;
		}


		/* Aguarda por um dos fd's para ser activado. Ou o accept, ou o
		stdin, ou o socket do predecessor ou o socket do sucessor: */
		counter = select(max_fd+1, &fdset, (fd_set*)NULL,(fd_set*)NULL,
			(struct timeval *)NULL);
		if(counter <= 0){
			perror("ddt:select");
			exit(0);
		}
		/* Alguem quer fazer ligacao TCP connosco. Ou somos o no de
		arranque e quer se juntar ao anel ou ele vai passar a ser o
		nosso predecessor: */
		if(FD_ISSET(transversal_data.t ,&fdset)){

			new_fd = accept(transversal_data.t,NULL,NULL);

			if(read_message_tcp(buffer,RCI_READSIZE,new_fd)<0){
				close(new_fd);
			}else{
				handle_new_message(buffer,&transversal_data,
					new_fd);
			}

		}
		if(transversal_data.peer_pred.socket>=0){
			if(FD_ISSET(transversal_data.peer_pred.socket,
			&fdset)){
				/* Predecessor mandou uma mensagem: */
				if(read_message_tcp(buffer,RCI_READSIZE,
				transversal_data.peer_pred.socket)<0){
					close(transversal_data.peer_pred.
						socket);
					transversal_data.peer_pred.id = -1;
					transversal_data.peer_pred.node[0] = 0;
					transversal_data.peer_pred.service[0]=
						0;
					transversal_data.peer_pred.socket = -1;
				}else{
					handle_pred_message(buffer,
						&transversal_data,-1);
				}
			}
		}
		if(transversal_data.peer_succ.socket >= 0){
			if(FD_ISSET(transversal_data.peer_succ.socket,
			&fdset)){
				//sucessor mandou uma mensagem
				if(read_message_tcp(buffer,RCI_READSIZE,
				transversal_data.peer_succ.socket)<0){
					close(transversal_data.peer_succ.
						socket);

					transversal_data.peer_succ.id = -1;
					transversal_data.peer_succ.node[0] = 0;
					transversal_data.peer_succ.service[0]=
						0;
					transversal_data.peer_succ.socket = -1;

				}else{
					handle_succ_message(buffer,
					&transversal_data,-1);
				}
			}
		}
		if(FD_ISSET(STDIN, &fdset)){
			if (interface(&transversal_data)){
				break;
			}else{
				printf(">");
				fflush(stdout);
			}
		}
	}
#ifdef RCIDEBUG1
	printf("Testing\n");
#endif
	exit(0);
}
