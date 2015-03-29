/** \file interface.c
Interface com o utilizador. Este ficheiro contém as funções relacionadas com
a interface com o utilizador.
*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "ring.h"
#include "net_common.h"
#include "fd_list.h"

/** Função de impressão de formato do comando <join> (curto). */
void print_join_s(){
	printf("\t[join x i] -> O utilizador pretende que o nó se junte ao anel"
	" x tomando i como identificador nesse anel. O nó só pode pertencer a"
	" um anel de cada vez.\n");
}

/** Função de impressão de formato do comando <join> (longo). */
void print_join_l(){
	printf("\t[join x i succi succi.IP succi.TCP] -> O utilizador pretende"
	" que o nó se junte ao anel x tomando i como identificador nesse anel e"
	" succi como o identificador do seu sucessor, localizado em"
	" (succi.IP succi.TCP).\n");
}

/** Função de impressão de formato do comando <search>. */
void print_search(){
	printf("\t[search k] -> O utilizador pretende saber o identificador e a"
	"localização do nó responsável pelo identificador k.\n");
}

/** Função de impressão dos comandos disponíveis. Esta função mostra os comandos
disponíveis para o utilizador. Os comandos sem parâmetros são hardcoded,
enquanto que os comandos com parâmetros estão descritos em funções individuais
de maneira a facilitar as mensagens de erro de formato.
*/
void print_ui(){
	printf("Escolha uma das instruções para executar:\n");

	print_join_s();
	print_join_l();

	printf("\t[leave] -> O utilizador pretende que o nó abandone o anel a"
	" que pertence.\n");

	print_search();

	printf("\t[show] -> Mostra ao utilizador o número do anel, o"
	" identificador do nó nesse anel, bem como os identificadores do seu"
	" sucessor e do seu predecessor.\n"
	"\t[help] -> A lista de comandos disponíveis é mostrada novamente.\n"
	"\t[exit] -> O utilizador fecha a aplicação.\n");
}

/** Função de impressão de erro nos comandos. Esta função mostra uma mensagem de
erro, informando o utilizador que o comando que inseriu é inválido.
*/
void print_error(){
	printf("Escolha um comando válido.\nUse [help] para ver uma lista dos"
		" comandos disponíveis.\n");
}

/** Função de análise dos comandos inseridos. Após detecção de dados para ser
lidos via stdin, esta função processa esses dados de maneira a executar o
comando inserido pelo utilizador.
*/
int interface(struct transversal_data *transversal_data){
	int num_com=0;

	char code = 0;

	char comands[6][256];
	char str[256];
	char message_to_send[256];
	
	
	char counter;
	
	fd_set to_fds;
	
	struct timeval timeout;
	
	
	connect_fd *aux;

	int anel_id, no_id;
	int no_proc_id;

	fgets(str, 256, stdin);
	num_com=sscanf(str, "%s %s %s %s %s %s", comands[0], comands[1],
		comands[2], comands[3], comands[4], comands[5]);


	if(num_com==6){
		if(strcmp(comands[0], "join")==0) code = 1;
	}
	//deve ser join
	else if(num_com==3){
		if(strcmp(comands[0], "join")==0) code = 2;
	}
	//Deve ser search
	else if(num_com==2){
		if(strcmp(comands[0], "search")==0) code = 3;
	}
	else if(num_com==1){
		if(strcmp(comands[0], "leave")==0){
			code = 5;
		}else if(strcmp(comands[0], "show")==0){
			code = 6;
		}else if(strcmp(comands[0], "exit")==0){
			code = 4;
		}else if(strcmp(comands[0], "help")==0){
			code = 7;
		}
	}


	switch(code){
		case 1: /* join com IPs */
			
			if(sscanf(comands[1], "%d", &anel_id)==1 &&
			sscanf(comands[2], "%d", &no_id)==1){
				if(no_id>=0 && no_id<64){
				//faz join
					join_spec_ring(comands[1],comands[2],
						comands[3],comands[4],
						comands[5],transversal_data);
				}else{
					printf("Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
				}
			}else{
				printf("Comando mal formatado."
					" Deve ser da forma:\n");
				print_join_s();
				print_join_l();
			}
			break;
			
			break;
		case 2: /* join só com número */
			if(sscanf(comands[1], "%d", &anel_id)==1 &&
			sscanf(comands[2], "%d", &no_id)==1){
				if(no_id>=0 && no_id<64){
				//faz join
					join_ring(comands[1],comands[2],
						transversal_data);
				}else{
					printf("Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
				}
			}else{
				printf("Comando mal formatado."
					" Deve ser da forma:\n");
				print_join_s();
				print_join_l();
			}
			break;
		case 3: /* Search */
			if(sscanf(comands[1], "%d", &no_proc_id)==1){
				if(no_proc_id>=0 && no_proc_id<64){
					//faz search
					if(transversal_data->ring==-1){
						printf("Nao está conectado em nenhum anel\n");
					}
					else if(verifica_se_responsavel(comands[1], transversal_data->id, transversal_data->peer_pred.id)){
						//se for ele responsavel entao responde logo ao utilizador
						printf("%d, %s, %s\n", transversal_data->id, transversal_data->ext_addr,
						transversal_data->startup_data.ringport);
					}else{
						aux=malloc(sizeof(connect_fd));
						aux->id = comands[1][0];
						aux->fd = 0;
						aux->next = NULL;
						transversal_data->primeiro = add_fd(aux,transversal_data->primeiro);
						//faz search do no que se procura enviando QRY j i ao succi
						sprintf(message_to_send, "QRY %d %s\n",transversal_data->id, comands[1]);
						write_message(message_to_send, transversal_data->peer_succ.socket);
					}
				}else{
					printf("Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
				}
			}else{
				printf("Comando mal formatado."
					" Deverá ser da forma:\n");
				print_search();
			}
			break;
		case 4: /* Exit */
			/* Sai do anel: */

		case 5: /* Leave */
			/* Sai do anel: */
			if(transversal_data->ring == -1){
				printf("Não há anel do qual sair.\n");
			}else if(transversal_data->serv_arranq &&
			transversal_data->peer_succ.socket==-1){
			
				
				for(counter=0;counter<5;counter++){
					FD_ZERO(&to_fds);
					FD_SET((*transversal_data).u,&to_fds);
		
					timeout.tv_sec = 3;
					timeout.tv_usec = 0;
					
					sprintf(message_to_send, "UNR %d",
						transversal_data->ring);
					
					sendto((*transversal_data).u,
						message_to_send,
						strlen(message_to_send),0,
						(*transversal_data).
							startup_data.destination
						,(*transversal_data).
							startup_data.dest_size);
	
					if(select((*transversal_data).u+1,
					&to_fds,NULL,NULL,&timeout)<1) continue;
	
					if(FD_ISSET((*transversal_data).u,&to_fds)){
						message_to_send[recvfrom(
							(*transversal_data).u,
							message_to_send,256,0,
							NULL,NULL)] ='\0';
#ifdef RCIDEBUG1
						printf("RCIDEBUG1: SA responds:"
						" <%s>",message_to_send);
#endif
						break;
					}
					printf("Timeout elapsed."
						" No contact from server.\n");
				}
				
				transversal_data->ring = -1;
				
				if(transversal_data->peer_pred.socket!=-1){
					sprintf(message_to_send,"CON %d %s %s\n"
						,transversal_data->peer_pred.id,
						transversal_data->
							peer_pred.node,
						transversal_data->
							peer_pred.service);
					write_message(message_to_send,
						transversal_data->
							peer_pred.socket);
					close(transversal_data->
						peer_pred.socket);
					
				}
				
				transversal_data->peer_pred.id = -1;
				transversal_data->peer_pred.node[0] = 0;
				transversal_data->peer_pred.service[0] = 0;
				transversal_data->peer_pred.socket = -1;

				transversal_data->peer_succ.id = -1;
				transversal_data->peer_succ.node[0] = 0;
				transversal_data->peer_succ.service[0] = 0;
				transversal_data->peer_succ.socket = -1;
				
			}else if(transversal_data->serv_arranq){
				
				for(counter=0;counter<5;counter++){
					FD_ZERO(&to_fds);
					FD_SET((*transversal_data).u,&to_fds);
		
					timeout.tv_sec = 3;
					timeout.tv_usec = 0;
					
					sprintf(message_to_send, "REG %d %d %s %s",
						transversal_data->ring,
						transversal_data->peer_succ.id,
						transversal_data->peer_succ.node,
						transversal_data->peer_succ.service);
					
					sendto((*transversal_data).u,message_to_send,
					strlen(message_to_send),0,
					(*transversal_data).startup_data.
						destination,
					(*transversal_data).startup_data.
						dest_size);
	
					if(select((*transversal_data).u+1,&to_fds,NULL,NULL,&timeout)<1) continue;
	
					if(FD_ISSET((*transversal_data).u,&to_fds)){
						message_to_send[recvfrom((*transversal_data).u,
							message_to_send,256,0,NULL,NULL)] ='\0';
#ifdef RCIDEBUG1
						printf("RCIDEBUG1: SA responds: <%s>",message_to_send);
#endif
						break;
					}
					printf("Timeout elapsed. No contact from server.\n");
				}
				
				sprintf(message_to_send, "BOOT\n");
				dprintf(transversal_data->peer_succ.socket,"%s",
					message_to_send);
			
				close(transversal_data->peer_succ.socket);
				sprintf(message_to_send,"CON %d %s %s\n"
						,transversal_data->peer_succ.id,
						transversal_data->
							peer_succ.node,
						transversal_data->
							peer_succ.service);
				write_message(message_to_send,transversal_data->
					peer_pred.socket);
				close(transversal_data->peer_pred.socket);
				
				transversal_data->peer_pred.id = -1;
				transversal_data->peer_pred.node[0] = 0;
				transversal_data->peer_pred.service[0] = 0;
				transversal_data->peer_pred.socket = -1;

				transversal_data->peer_succ.id = -1;
				transversal_data->peer_succ.node[0] = 0;
				transversal_data->peer_succ.service[0] = 0;
				transversal_data->peer_succ.socket = -1;
			}else{
				close(transversal_data->peer_succ.socket);
				sprintf(message_to_send,"CON %d %s %s\n"
						,transversal_data->peer_succ.id,
						transversal_data->
							peer_succ.node,
						transversal_data->
							peer_succ.service);
				write_message(message_to_send,transversal_data->
					peer_pred.socket);
				close(transversal_data->peer_pred.socket);
				
				transversal_data->peer_pred.id = -1;
				transversal_data->peer_pred.node[0] = 0;
				transversal_data->peer_pred.service[0] = 0;
				transversal_data->peer_pred.socket = -1;

				transversal_data->peer_succ.id = -1;
				transversal_data->peer_succ.node[0] = 0;
				transversal_data->peer_succ.service[0] = 0;
				transversal_data->peer_succ.socket = -1;

			}			
			if(code == 4) return 1;

			break;
		case 6: /* Show */
			printf("Anel: %d / ID: %d / Sucessor: %d / Predecessor: %d\n", transversal_data->ring, transversal_data->id, transversal_data->peer_succ.id, transversal_data->peer_pred.id);
			break;
		case 7: /* Help */
			/* Mostra de novo os comandos disponíveis: */
			print_ui();
			break;
		default: /* Erro: */
			print_error();
			break;
	}

	return 0;
}
