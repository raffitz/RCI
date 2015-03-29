/** \file message_handler.c
Tratamento de mensagens recebidas. Este ficheiro contém as implementações das
funções de tratamento de mensagens recebidas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "common.h"
#include "ring.h"
#include "net_tcp.h"
#include "net_common.h"



/** Função que preenche a estrutura de informação do par predecessor. */
void preenche_pred_info(struct transversal_data * transversal_data, char* id,
	char* ip, char* porto, int new_fd)
{
	sscanf(id, "%d", &transversal_data->peer_pred.id);
	strcpy(transversal_data->peer_pred.node, ip);
	strcpy(transversal_data->peer_pred.service, porto);
	transversal_data->peer_pred.socket=new_fd;

	return;
}

/** Função que preenche a estrutura de informação do par sucessor. */
void preenche_succ_info(struct transversal_data * transversal_data, char* id,
	char* ip, char* porto, int new_fd)
{
	sscanf(id, "%d", &transversal_data->peer_succ.id);
	strcpy(transversal_data->peer_succ.node, ip);
	strcpy(transversal_data->peer_succ.service, porto);
	transversal_data->peer_succ.socket=new_fd;

	return;
}



/** Verifica se a mensagem recebida de um novo nó é válida e identifica-a. A
identificação é feita através do valor devolvido pela função, que pode ser um dos seguintes:
	0-erro / 1-ID / 2-NEW
*/
int check_new_message(char message[6][256], int num_words){
	int new_pred;
	int procu_id;
	
	/* Deve ser new ou connect ou success: */
	if(num_words==4){
		if(strcmp(message[0], "NEW")==0){
			if(sscanf(message[1], "%d", &new_pred)==1){
				if(new_pred>=0 && new_pred<64){
					return(2);
				}else{
#ifdef RCIDEBUG2
					printf("[R] Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
#endif
					return(0);
				}
			}else{
#ifdef RCIDEBUG2
				printf("[R] Mensagem mal formatada!\n");
#endif
				return(0);
			}
		}else{
#ifdef RCIDEBUG2
			printf("[R] Mensagem incorrecta.\n");
#endif
			return(0);
		}
	}else if(num_words==2){/* Deve ser tentativa de um nó se juntar ao anel: */
		if(strcmp(message[0], "ID")==0){
			if(sscanf(message[1], "%d", &procu_id)==1){
				if(procu_id>=0 && procu_id<64){
					return(1);
				}else{
#ifdef RCIDEBUG2
					printf("[R] Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
#endif
					return(0);
				}
			}else{
#ifdef RCIDEBUG2
				printf("[R] Mensagem mal formatada!\n");
#endif
				return(0);
			}
		}else{
#ifdef RCIDEBUG2
			printf("[R] Mensagem incorrecta.\n");
#endif
			return(0);
		}
	}else{ /* Mensagem esta mal formatada: */
#ifdef RCIDEBUG2
		printf("[R] Está tudo mal!\n");
#endif
		/* Sou particularmente adepto desta mensagem de erro */
		return(0);
	}

}

/** Função que age consoante a mensagem recebida de um novo nó. */
void handle_new_message(char* buffer,struct transversal_data * transversal_data,
int new_fd){
	char message[6][256];
	char response[256];
	int num_words, option;
	
	connect_fd *aux;
	
	int fd_aux;

	num_words = sscanf(buffer, "%s %s %s %s %s %s", message[0],message[1],
		message[2],message[3],message[4],message[5]);
	
	/* Redundante: */
	if(message[num_words][strlen(message[num_words])-1] == '\n'){
		message[num_words][strlen(message[num_words])-1] = '\0';
	}
	
	option = check_new_message(message, num_words);

	switch (option){
		case 1:/* ID */
			if( (*transversal_data).serv_arranq == 0){
				close(new_fd);
				break;
			}
			if(verifica_se_responsavel(message[1],transversal_data->
			id,transversal_data->peer_pred.id)){
				/* Se for ele responsável então responde logo
				ao novo nó com a resposta adequada: */
				sprintf(response, "SUCC %d %s %s\n",
					transversal_data->id,
					transversal_data->ext_addr,
					transversal_data->
						startup_data.ringport);
				write_message(response, new_fd);
				close(new_fd);
			}else{
				/* Faz search do nó que se procura enviando
				QRY j i ao succ */
				
				aux = malloc(sizeof(connect_fd));
				(*aux).id = message[1][0];
				(*aux).fd = new_fd;
				(*aux).next = NULL;
				transversal_data->primeiro = add_fd(aux,
					transversal_data->primeiro);
				
				sprintf(response, "QRY %d %s\n",
					transversal_data->id, message[1]);
					write_message(response,
						transversal_data->
							peer_succ.socket);
			}
			break;

		case 2:/* NEW */
			/* Mudar o predecessor para i */
			if(transversal_data->peer_pred.socket==-1){
				preenche_pred_info(transversal_data,message[1],
					message[2], message[3], new_fd);
				
				if(transversal_data->peer_succ.socket==-1){
					sprintf(response,"NEW %d %s %s\n",
						transversal_data->id,
						transversal_data->ext_addr,
						transversal_data->
							startup_data.ringport);
					
					fd_aux = connect_tcp(message[2],
						message[3]);
					
					write_message(response,fd_aux);
					
					preenche_succ_info(transversal_data,
						message[1],message[2],
						message[3], fd_aux);
				
				}
				
			}else{
				sprintf(response, "CON %s %s %s\n",
					message[1], message[2], message[3]);
				write_message(response,
					transversal_data->peer_pred.socket);
				close(transversal_data->peer_pred.socket);
				preenche_pred_info(transversal_data,message[1],
					message[2], message[3], new_fd);
			}
			break;
		default:/* Erro */
			close(new_fd);
			break;
	}
	return;
}

/** Verifica se a mensagem recebida dum predecessor é válida e identifica-a. A
identificação é feita através do valor devolvido pela função, que pode ser um
dos seguintes:
	0-erro / 1-QRY / 2-BOOT
*/
int check_pred_message(char message[6][256], int num_words){
	int ini_id,procu_id;
	
	/* Deve ser procura QRY: */
	if(num_words==3){
		if(strcmp(message[0], "QRY")==0){
			if(sscanf(message[1], "%d", &ini_id)==1 &&
			sscanf(message[2], "%d", &procu_id)==1){
				if((ini_id>=0 && ini_id<64) ||
				(ini_id>=0 && ini_id<64)){
					return(1);
				}else{
#ifdef RCIDEBUG2
					printf("[R] Identificador do nó fora de"
					" alcance. Deve estar entre 0 e 63.\n");
#endif
					return(0);
				}
			}else{
#ifdef RCIDEBUG2
				printf("[R] Mensagem mal formatada!\n");
#endif
				return(0);
			}
		}else{
#ifdef RCIDEBUG2
			printf("[R] Mensagem incorrecta.\n");
#endif
			return(0);
		}
	}else if(num_words==1){
		if(strcmp(message[0], "BOOT")==0){
			return(2);
		}
		else{
#ifdef RCIDEBUG2
			printf("[R] Mensagem incorrecta.\n");
#endif
			return(0);
		}
	}else{ /* Mensagem esta mal formatada: */
#ifdef RCIDEBUG2
		printf("[R] Está tudo mal!\n");
		/* Sou particularmente adepto desta mensagem de erro */
#endif
		return(0);
	}

}

/** Função que responde às mensagens recebidas do predecessor. */
void handle_pred_message(char* buffer,
struct transversal_data * transversal_data, int new_fd){
	char message[6][256];
	char response[256];
	int num_words, option;


	num_words = sscanf(buffer, "%s %s %s %s %s %s", message[0],message[1],
		message[2],message[3],message[4],message[5]);
	
	/* Em princípio, redundante: */
	if(message[num_words][strlen(message[num_words])-1] == '\n'){
		message[num_words][strlen(message[num_words])-1] = '\0';
	}
	option = check_pred_message(message, num_words);

	switch (option){
		case 1:/* QRY */
			/* Verifica se é responsável pelo nó.
			Depois responde adequadamente. */
			if(verifica_se_responsavel(message[2],transversal_data->
			id,transversal_data->peer_pred.id)){
			/* Se for responsável responde: */
				sprintf(response, "RSP %s %s %d %s %s\n",
					message[1], message[2],
					transversal_data->id,
					transversal_data->ext_addr,
					transversal_data->
						startup_data.ringport);
				write_message(response, transversal_data->
					peer_pred.socket);
			}else{
			/* Se não for responsável passa a mensagem a succ: */
				sprintf(buffer,"%s\n",buffer);
				write_message(buffer, transversal_data->
					peer_succ.socket);
			}
			break;

		case 2:/* BOOT */
			/* Muda a variavel que diz se somos no de arranque ou
				nao. */
			transversal_data->serv_arranq = 1;
			break;
		default:/* Erro */
			/* Squash da mensagem anómala */
#ifdef RCIDEBUG2
				printf("pred says\"%s\"\n",buffer);
#endif
			break;
	}
	return;
}

/** Verifica se a mensagem recebida do sucessor é válida e identifica-a. A
identificação é feita através do valor devolvido pela função, que pode ser um
dos seguintes:
	0-erro / 1-RSP / 2-CON
*/
int check_succ_message(char message[6][256], int num_words){
	int ini_id, procu_id, resp_id;
	int new_succ;
	
	/* Deve ser RSP: */
	if(num_words==6){
		if(strcmp(message[0], "RSP")==0){
			if(sscanf(message[1], "%d", &ini_id)==1 &&
			sscanf(message[2], "%d", &procu_id)==1 &&
			sscanf(message[3], "%d", &resp_id)==1){
				if((ini_id>=0 && ini_id<64) ||
				(procu_id>=0 && procu_id<64) ||
				(resp_id>=0 && resp_id<64)){
					return(1);
				}else{
#ifdef RCIDEBUG2
					printf("[R] Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
#endif
					return(0);
				}
			}else{
#ifdef RCIDEBUG2
				printf("[R] Mensagem mal formatada!\n");
#endif
				return(0);
			}
		}else{
#ifdef RCIDEBUG2
			printf("[R] Mensagem incorrecta.\n");
#endif
			return(0);
		}
	}else if(num_words==4){
		if(strcmp(message[0], "CON")==0){
			if(sscanf(message[1], "%d", &new_succ)==1){
				if(new_succ>=0 && new_succ<64){
					return(2);
				}else{
#ifdef RCIDEBUG2
					printf("[R] Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
#endif
					return(0);
				}
			}else{
#ifdef RCIDEBUG2
				printf("[R] Mensagem mal formatada!\n");
#endif
				return(0);
			}
		}else{
#ifdef RCIDEBUG2
			printf("[R] Mensagem incorrecta.\n");
#endif
			return(0);
		}
	}else{
		/* Mensagem esta mal formatada: */
#ifdef RCIDEBUG2
		printf("[R] Está tudo mal!\n");
		/* Sou particularmente adepto desta mensagem de erro */
#endif
		return(0);
	}

}



/** Função que responde às mensagens recebidas do sucessor. */
void handle_succ_message(char* buffer,struct transversal_data *transversal_data,
int new_fd){
	char message[6][256];
	int num_words, option;
	char response[128];
	int origin;
	
	connect_fd * aux;
	int fd_aux;

	num_words = sscanf(buffer, "%s %s %s %s %s %s", message[0],message[1],
		message[2],message[3],message[4],message[5]);
	
	/* Redundante: */
	if(message[num_words][strlen(message[num_words])-1] == '\n'){
		message[num_words][strlen(message[num_words])-1] = '\0';
	}
	option = check_succ_message(message, num_words);

	switch (option){
		case 1:/* RSP */
		/* Se message[1] (no que fez pesquisa) for o mesmo que o nó
		actual então responde. Se não, passa a mensagem. */
			sscanf(message[1],"%d",&origin);
			if(origin==transversal_data->id){
				/* Se a pesquisa foi iniciada pelo próprio,
				então a resposta é dada ao utilizador ou ao nó
				que deseja aderir.*/
				aux=search_fd(message[2],
					transversal_data->primeiro, &fd_aux);
				if(fd_aux==0){
					printf("Resposta: %s, %s, %s\n",
						message[3], message[4],
						message[5]);
				}else{
					sprintf(response, "SUCC %s %s %s\n",
					message[3], message[4], message[5]);
					write_message(response, fd_aux);
					close(fd_aux);
				}
				transversal_data->primeiro=remove_fd(aux,
					transversal_data->primeiro);
			}else{
				/* Se a mensagem não foi iniciada pelo próprio,
				retransmite a mensagem para pred. */
				sprintf(buffer,"%s\n",buffer);
				write_message(buffer,
					transversal_data->peer_pred.socket);
			}
			break;

		case 2:/* CON */
			/* Tenho que me ligar ao nó i */
			
			sprintf(response, "NEW %d %s %s\n",transversal_data->id,
				transversal_data->ext_addr,transversal_data->
					startup_data.ringport);
			
			fd_aux = connect_tcp(message[2], message[3]);
			
			write_message(response,fd_aux);
			
			if(transversal_data->peer_succ.socket != -1){
				close(transversal_data->peer_succ.socket);
			}
			
			preenche_succ_info(transversal_data,message[1],
				message[2], message[3], fd_aux);
				
			
			
			
			break;

		
		default:/* Erro */
			break;
	}
	return;
}

