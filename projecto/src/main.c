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
#include "net_common.h"
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
	char buffer[256];

	/* Variáveis para o accept: */
	struct sockaddr_storage address;
	socklen_t length;
	char caddr[40];
	char cport[16];

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


	//inicializa lista de fd's
	FD_ZERO(&transversal_data.fds);
	FD_SET(STDIN, &transversal_data.fds);
	max_fd=STDIN;



	while(1){
		/*aguarda por um dos fd's para ser activado. Ou o accept, ou o stdin, ou o socket do predecessor
		ou o socket do sucessor*/
		counter = select(max_fd+1, &transversal_data.fds, (fd_set*)NULL, (fd_set*)NULL, (struct timeval *)NULL);
		if(counter <= 0){
			exit(1);
		}
		/*Alguem quer fazer ligacao TCP connosco. Ou somos o no de arranque e quer se juntar
		ao anel ou ele vai passar a ser o nosso predecessor*/
		if(FD_ISSET(transversal_data.t ,&transversal_data.fds)){
			//alguem esta-se a ligar a nos (select)
			int new_fd;
			//fazer accept e conectar-se

			length = sizeof(struct sockaddr_storage);

			new_fd = accept(transversal_data.t,
				(struct sockaddr*)&address,&length);

			getIP((struct sockaddr*) &address,length,caddr,cport);

			read_message_tcp(buffer, new_fd);
#ifdef RCIDEBUG1
			printf("RCIDEBUG1: <%s : %s> says %s\n",caddr,cport,
				buffer);
#endif
			trata_mensagem(buffer, &transversal_data, new_fd);
		}
		if(transversal_data.peer_pred.socket>=0){
			if(FD_ISSET(transversal_data.peer_pred.socket ,&transversal_data.fds)){
				//predecessor mandou uma mensagem
				read_message_tcp(buffer, transversal_data.peer_pred.socket);
				trata_mensagem(buffer, &transversal_data, -1);
			}
		}
		if(transversal_data.peer_succ.socket >= 0){
			if(FD_ISSET(transversal_data.peer_succ.socket ,&transversal_data.fds)){
				//sucessor mandou uma mensagem
				read_message_tcp(buffer, transversal_data.peer_pred.socket);
				trata_mensagem(buffer, &transversal_data, -1);
			}
		}
		if(FD_ISSET(STDIN, &transversal_data.fds)){
			if (interface(&transversal_data)){
				break;
			}else{
				printf(">");
			}
		}
	}
#ifdef RCIDEBUG1
	printf("Testing\n");
#endif
	exit(0);
}
