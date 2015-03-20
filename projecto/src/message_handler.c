/** \file message_handler.c
Tratamento de mensagens recebidas. Este ficheiro contém as implementações das
funções de tratamento de mensagens recebidas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"

#define RCI_MAXNR 64

/** Função de cálculo da distância entre dois nós. */
int dist(int ele, int eu){
	if (eu>=ele){
		return (eu-ele);
	}else{
		return(RCI_MAXNR+eu-ele);
	}

}

/** Função de verificação da responsabilidade de um nó. 1-> responsavel / 0->nao responsavel*/
int verifica_se_responsavel(char * c, int eu_id, int predi_id){

	int id;
	if(predi_id>=0){
		sscanf(c, "%d", &id);
		if(dist(id, eu_id)<dist(id, predi_id)){
			return 1;
		}else{
			return 0;
		}
	}else{
		return 1;
	}
}

/** Função que lê dum file descriptor (socket) uma mensagem*/
void read_message_tcp(char* buffer, int fd){
	int flag = 0;
	int nread;
	int num_char = 0;

//so le um char de cada vez para ir sempre verificando quando encontra o '\n'
	while(flag==0){
		nread=read(fd, buffer, 1);
		if(nread==-1){
			printf("Erro ao ler do file descriptor\n");
			return;
		}else if((nread==0) || (buffer[num_char]=='\n'){
			buffer[num_char+1]='\0';
			break;
		}
		num_char++;
		buffer++;
	}
	return;
}

void write_message(char * buffer, int fd){
	int nleft=strlen(buffer);
	int nwritten;
	if(fd!=-1){
		while(nleft>0){
			nwritten=write(fd,buffer,nleft);
			if(nwritten<=0){
				//error
			}
			nleft-=nwritten;
			ptr+=nwritten;
		}
	}
	return;
}

/** Verifica se a mensagem recebida é válida e identifica-a. A identificação
é feita através do valor devolvido pela função, que pode ser um dos seguintes:
	0-erro / 1-RSP / 2-NEW / 3-CON / 4-SUCC / 5-QRY / 6-ID / 7-BOOT / 8-BRSP
*/
int check_message(char** message, int num_words){

	int ini_id, procu_id, resp_id;
	int anel_id, no_arr_id;

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
					printf("[R] Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
					return(0);
				}
			}else{
				printf("[R] Mensagem mal formatada!\n");
				return(0);
			}
		}else{
			printf("[R] Mensagem incorrecta.\n");
			return(0);
		}
	}
	/* Deve ser o servidor de arranque a responder que o anel pedido não
	está vazio com BRSP: */
	if(num_words==5){
		if(strcmp(message[0], "BRSP")==0){
			if(sscanf(message[1], "%d", &anel_id)==1 &&
			sscanf(message[2], "%d", no_arr_id)){
				if(no_arr_id>=0 && no_arr_id<64){
					return(8);
				}else{
					printf("[R] Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
					return(0);
				}
			}else{
				printf("[R] Mensagem mal formatada!\n");
				return(0);
			}
		}else{
			printf("[R] Mensagem incorrecta.\n");
			return(0);
		}
	}
	/* Deve ser new ou connect ou success: */
	else if(num_words==4){
		if(strcmp(message[0], "NEW")==0){
			int new_pred;
			if(sscanf(message[1], "%d", &new_pred)==1){
				if(new_pred>=0 && new_pred<64){
					return(2);
				}else{
					printf("[R] Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
					return(0);
				}
			}else{
				printf("[R] Mensagem mal formatada!\n");
				return(0);
			}
		}
		else if(strcmp(message[0], "CON")==0){
			int new_succ;
			if(sscanf(message[1], "%d", &new_succ)==1){
				if(new_succ && new_succ<64){
					return(3);
				}else{
					printf("[R] Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
					return(0);
				}
			}else{
				printf("[R] Mensagem mal formatada!\n");
				return(0);
			}
		}
		else if(strcmp(message[0], "SUCC")==0){
			int resp_id;
			if(sscanf(message[1], "%d", &resp_id)==1){
				if(resp_id>=0 && resp_id<64){
					return(4);
				}else{
					printf("[R] Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
					return(0);
				}
			}else{
				printf("[R] Mensagem mal formatada!\n");
				return(0);
			}
		}
		else{
			printf("[R] Mensagem incorrecta.\n");
			return(0);
		}
	}
	/* Deve ser procura QRY: */
	else if(num_words==3){
		int ini_id, procu_id;
		if(strcmp(message[0], "QRY")==0){
			if(sscanf(message[1], "%d", &ini_id)==1 &&
			sscanf(message[2], "%d", &procu_id)==1){
				if((ini_id>=0 && ini_id<64) ||
				(ini_id>=0 && ini_id<64)){
					return(5);
				}else{
					printf("[R] Identificador do nó fora de"
					" alcance. Deve estar entre 0 e 63.\n");
					return(0);
				}
			}else{
				printf("[R] Mensagem mal formatada!\n");
				return(0);
			}
		}else{
			printf("[R] Mensagem incorrecta.\n");
			return(0);
		}
	}
	/* Deve ser tentativa de um nó se juntar ao anel: */
	else if(num_words==2){
		int procu_id;

		if(strcmp(message[0], "ID")==0){
			if(sscanf(message[1], "%d", &procu_id)==1){
				if(procu_id && procu_id){
					return(6);
				}else{
					printf("[R] Identificador do nó fora do"
					" alcance. Deve estar entre 0 e 63.\n");
					return(0);
				}
			}else{
				printf("[R] Mensagem mal formatada!\n");
				return(0);
			}
		}else{
			printf("[R] Mensagem incorrecta.\n");
			return(0);
		}
	}
	/* Deve ser para passar a ser nó de arranque: */
	else if(num_words==1){
		if(strcmp(message[0], "BOOT")==0){
			return(7);
		}
		else{
			printf("[R] Mensagem incorrecta.\n");
			return(0);
		}
	}
	/* Mensagem esta mal formatada: */
	else{
		printf("[R] Está tudo mal!\n");
		/* Sou particularmente adepto desta mensagem de erro */
		return(0);
	}

}

/** Função que preenche a estrutura de informação do par predecessor. */
void preenche_predi_info(struct transversal_data * transversal_data, char* id,
	char* ip, char* porto, int new_fd)
{
	sscanf(id, "%d", &transversal_data->peer_pred.id);
	strcpy(transversal_data->peer_pred.node, ip);
	strcpy(transversal_data->peer_pred.service, porto);
	transversal_data->peer_pred.socket=new_fd;

	return;
}

/** Função que preenche a estrutura de informação do par predecessor. */
void trata_mensagem(char* buffer, struct transversal_data * transversal_data, int new_fd){
	char message[6][256];
	int num_words, option;
	char message_to_send[128];
	int eu_id, predi_id;

	eu_id=transversal_data->id;
	predi_id=transversal_data->peer_pred.id;

	num_words = sscanf(buffer, "%s %s %s %s %s %s", message[0],message[1],
		message[2],message[3],message[4],message[5]);
	message[num_words][strlen(message[num_words])-1] = '\0';
	option = check_message((char**)message, num_words);

	switch (option){
		case 1:/* RSP */
			/* Se message[1] (no que fez pesquisa) for o mesmo que o nó actual então responde.
			Se não, passa a mensagem. */
			if(atoi(message[1])==transversal_data->id){
				/* Se a pesquisa foi iniciada pelo próprio,
				então a resposta é dada ao utilizador ou ao nó
				que deseja aderir.*/
				connect_fd * aux;
				int fd_aux;
				aux=search_fd(message[2], transversal_data->primeiro, &fd_aux);
				if(fd_aux==0){
					printf("\n%s, %s, %s\n", message[3], message[4],
					message[5]);
				}else{
					sprintf(message_to_send, "SUCC %s %s %s\n",
					message[3], message[4], message[5]);
					write_message(message_to_send, fd_aux);
				}
				transversal_data.primeiro=remove_fd(aux, transversal_data.primeiro);
			}else{
				/* Se a mensagem não foi iniciada pelo próprio,
				retransmite a mensagem para predi. */
				dprintf(transversal_data->peer_pred.socket,
					"%s",buffer);;
			}
			break;

		case 2:/* NEW */
			/* Mudar o predecessor para i */
			if(transversal_data->peer_pred.fd==-1){
				preenche_predi_info(transversal_data,  message[1],
					message[2], message[3], new_fd);
				sscanf(message[1], "%d", &transversal_data->peer_succ.id);
				strcpy(transversal_data->peer_succ, message[2]);
				strcpy(transversal_data->peer_succ.service, message[3]);
				transversal_data->peer_succ.socket=new_fd;
			}else{
				sprintf(message_to_send, "CON %s %s %s\n",
					message[1], message[2], message[3]);
				preenche_predi_info(transversal_data,  message[1],
					message[2], message[3], new_fd);
			}
			break;

		case 3:/* CON */
			/* Tenho que me ligar ao nó i */
			// /!\ connect_tcp(message[2], message[3]);
			/* TODO falta guardar o nº do FD */
			break;

		case 4:/* SUCC */
			/* Informar o no que quer aderir que encontramos o seu
			sucessor: */
			if(atoi(message[1])==transversal_data->id){
				/* Temos que escolher outro id */
			}else{
				/* Liga-se a L (message[2] e message[3]) */
				connect_tcp(message[2], message[3]);
				sprintf(message_to_send, "NEW %d %s %s\n",
					transversal_data->id,
					transversal_data->ext_addr,
					transversal_data->
						startup_data.ringport);
				dprintf(transversal_data->peer_succ.socket,"%s",
					message_to_send);
			}
			break;

		case 5:/* QRY */
			/* Verifica se ele e responsavel pelo no.
			Depois responde adequadamente. */

			if(verifica_se_responsavel(message[2],eu_id,predi_id)){
			/* Se for responsavel responde: */
				sprintf(message_to_send, "RSP %s %s %s %s %s\n",
					message[1], message[2],
					transversal_data->id,
					transversal_data->ext_addr,
					transversal_data->
						startup_data.ringport);
				write_message(message_to_send, transversal_data->peer_pred.socket);
			}else{
			/*Se nao for responsavel passa a mensagem a succ */
				write_message(message_to_send, transversal_data->peer_succ.socket);
			}
			break;

		case 6:/* ID */
			if(verifica_se_responsavel(message[1],eu_id,predi_id)){
				/* Se for ele responsavel entao responde logo
				ao novo no com a resposta adequada: */
				sprintf(message_to_send, "SUCC %d %s %s\n",
					transversal_data->id, transversal_data->ext_addr, transversal_data->startup_data.ringport);
				write_message(message_to_send, fd);
			}else{
				/* Faz search do no que se procura enviando
				QRY j i ao succ */
				sprintf(message_to_send, "QRY %d %s\n",
					transversal_data->id, message[1]);
					write_message(message_to_send, transversal_data->peer_succ.socket);
					connect_fd con_fd;
					con_fd.fd=new_fd;
					transversal_data.primeiro = add_fd(&con_fd, transversal_data.primeiro);
			}
			break;

		case 7:/* BOOT */
			/* Muda a variavel que diz se somos no de arranque ou
				nao. */
			transversal_data->serv_arranq = 1;
			break;

		case 8:/* BRSP */
			/* Conecta-se por TCP ao no de arranque e envia-lhe a
				mensagem ID i */
			// /!\ connect_tcp(message[3], message[4]);
			/* TODO Falta guardar o FD */
			sprintf(message_to_send, "ID %d", transversal_data->id);

			break;
		default:/* Erro */
			break;
	}
	return;
}
