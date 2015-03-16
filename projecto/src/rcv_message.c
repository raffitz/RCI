#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
/*Verificar se a mensagem recebida é válida e identifica qual é:
	0-erro / 1-RSP / 2-NEW / 3-CON / 4-SUCC / 5-QRY / 6-ID / 7-BOOT
*/
int check_message(char** message){
	//deve ser RSP
	if(num_words==6){
		int ini_id, procu_id, resp_id;
		if(strcmp(message[0], "RSP")==0){
			if(sscanf(message[1], "%d", &ini_id)==1 && sscanf(message[2], "%d", &procu_id)==1 && sscanf(message[3], "%d", &resp_id)==1){
				if((ini_id>=0 && ini_id<64) || (procu_id>=0 && procu_id<64) || (resp_id>=0 && resp_id<64)){
					return(1);
				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
					return(0);
				}
			}else{
				printf("\nMensagem mal formatada\n");
				return(0);
			}
		}else{
			printf("\nMensagem incorrecta\n");
			return(0);
		}
	}
	//deve ser new ou connect ou success
	else if(num_words==4){
		if(strcmp(message[0], "NEW")==0){
			int new_pred;
			if(sscanf(message[1], "%d", &new_pred)==1){
				if(new_pred>=0 && new_pred<64){
					return(2);
				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
					return(0);
				}
			}else{
				printf("\nMensagem mal formatada!\n");
				return(0);
			}
		}
		else if(strcmp(message[0], "CON")==0){
			int new_succ;
			if(sscanf(message[1], "%d", &new_succ)==1){
				if(new_succ && new_succ<64){
					return(3);
				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
					return(0);
				}
			}else{
				printf("\nMensagem mal formatada!\n");
				return(0);
			}
		}
		else if(strcmp(message[0], "SUCC")==0){
			int resp_id;
			if(sscanf(message[1], "%d", &resp_id)==1){
				if(resp_id>=0 && resp_id<64){
					return(4);
				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
					return(0);
				}
			}else{
				printf("\nMensagem mal formatada!\n");
				return(0);
			}
		}
		else{
			printf("\nMensagem incorrecta");
			return(0);
		}
	}
	//deve ser procura QRY
	else if(num_words==3){
		int ini_id, procu_id;
		if(strcmp(message[0], "QRY")==0){
			if(sscanf(message[1], "%d", &ini_id)==1 && sscanf(message[2], "%d", &procu_id)==1){
				if((ini_id>=0 && ini_id<64) || (ini_id>=0 && ini_id<64)){
					return(5);
				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
					return(0);
				}
			}else{
				printf("\nMensagem mal formatada!\n");
				return(0);
			}
		}else{
			printf("\nMensagem incorrecta!\n");
			return(0);
		}
	}
	//Deve ser tentativa de um n� se juntar ao anel
	else if(num_words==2){
		int procu_id;

		if(strcmp(message[0], "ID")==0){
			if(sscanf(message[1], "%d", &procu_id)==1){
				if(procu_id && procu_id){
					return(6);
				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
					return(0);
				}
			}else{
				printf("\nMensagem mal formatada!\n");
				return(0);
			}
		}else{
			printf("\nMensagem incorrecta!\n");
			return(0);
		}
	}
	//deve ser para passar a ser n� de arranque
	else if(num_words==1){
		if(strcmp(message[0], "BOOT")==0){
			return(7);
		}
		else{
			printf("\nMensagem incorrecta\n");
			return(0);
		}
	}
	//mensagem esta mal formatada
	else{
		printf("\nEst� tudo mal!\n");
		return(0);
	}


	return valid;
}

void write_message_tcp(char * string, int socket){
	int nwritten, nleft;

	nleft=strlen(string);

	while(nleft>0){
		nwritten=write(socket, string, nleft);
		if(nwritten<=0) exit(1);//error
		nleft-=nwritten;
		string+=nwritten;
	}

	return;
}

char* read_message_tcp(int socket){
	char meesage[124];
	/*while(nleft>0){
		nread=read(fd,ptr,nleft);
		if(nread==-1)exit(1);//error
		else if(nread==0)break;//closed by peer nleft-=nread;
		ptr+=nread;
	}
	nread=nbytes-nleft;*/
	return message;
}
//Faz a pesquisa de um nó e recebe a mensagem de resposta
char* pesquisa_no(char* no_proc){
	char message_to_send[124];
	char * message_rcvd;

	sprintf(message_to_send, "QRY %s %s\n", (string)transversal_data.id, no_proc);
	write_message_tcp(message_to_send, transversal_data.peer_succi.socket);
	message_rcvd = read_message_tcp(transversal_data.peer_succi.socket);

	return message_rcvd;
}

void recv_message(char* buffer){
	char message[6][256];
	int num_words, option;
	char message_to_send[124];

	num_words = sscanf(buffer, "%s %s %s %s %s %s", message[0],message[1],message[2],message[3],message[4],message[5]);

	option = check_message(message);

	switch (option){
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
	}
	//deve ser RSP
	if(num_words==6){
		int ini_id, procu_id, resp_id;
		if(strcmp(message[0], "RSP")==0){
			if(sscanf(message[1], "%d", &ini_id)==1 && sscanf(message[2], "%d", &procu_id)==1 && sscanf(message[3], "%d", &resp_id)==1){
				if((ini_id>=0 && ini_id<64) || (procu_id>=0 && procu_id<64) || (resp_id>=0 && resp_id<64)){
				//se ini_id for o mesmo que o present no entao responde. Se nao passa a mensagem.

				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
				}
			}else{
				printf("\nMensagem mal formatada\n");
			}
		}else{
			printf("\nMensagem incorrecta\n");
		}
	}
	//deve ser new ou connect ou success
	else if(num_words==4){
		if(strcmp(message[0], "NEW")==0){
			int new_pred;
			if(sscanf(message[1], "%d", &new_pred)==1){
				if(new_pred>=0 && new_pred<64){
					//mudar o predecessor para i
				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
				}
			}else{
				printf("\nMensagem mal formatada!\n");
			}
		}
		else if(strcmp(message[0], "CON")==0){
			int new_succ;
			if(sscanf(message[1], "%d", &new_succ)==1){
				if(new_succ && new_succ<64){
					//tenho que me conectar com o n� i
				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
				}
			}else{
				printf("\nMensagem mal formatada!\n");
			}
		}
		else if(strcmp(message[0], "SUCC")==0){
			int resp_id;
			if(sscanf(message[1], "%d", &resp_id)==1){
				if(resp_id>=0 && resp_id<64){
					//informar o no que quer aderir que encontramos o seu succi
				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
				}
			}else{
				printf("\nMensagem mal formatada!\n");
			}
		}
		else{
			printf("\nMensagem incorrecta");
		}
	}
	//deve ser procura QRY
	else if(num_words==3){
		int ini_id, procu_id;
		if(strcmp(message[0], "QRY")==0){
			if(sscanf(message[1], "%d", &ini_id)==1 && sscanf(message[2], "%d", &procu_id)==1){
				if((ini_id>=0 && ini_id<64) || (ini_id>=0 && ini_id<64)){
				//Vai ver se ele � responsavel pelo no. Depois responde adequadamente.
					if(verifica_se_responsavel(procu_id)){
					//Se for responsavel responde
						sprintf(message_to_send, "RSP %s %s %s %s %s\n", message[1], message[2], transversal_data.id, transversal_data.ext_addr, transversal_data.startup_data.ringport);
						write_message_tcp(message_to_send, transversal_data.peer_pred.socket);
					}else{
					//Se nao for responsavel passa a mensagem a succi
						write_message_tcp(buffer, transversal_data.peer_succi.socket);
					}
				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
				}
			}else{
				printf("\nMensagem mal formatada!\n");
			}
		}else{
			printf("\nMensagem incorrecta!\n");
		}
	}
	//Deve ser tentativa de um n� se juntar ao anel
	else if(num_words==2){
		int procu_id;

		if(strcmp(message[0], "ID")==0){
			if(sscanf(message[1], "%d", &procu_id)==1){
				if(procu_id && procu_id){
				//faz search do no que se procura enviando QRY j i ao succi
				char * mess_rcvd;

				mess_rcvd = pesquisa_no(message[1]);
				sprintf(message_to_send, "SUCC %s", message_rcvd+8);
				//responde ao novo no i a quem ele se deve ligar
				write_message_tcp(message_to_send, transversal_data.socket_with_new_node);

				}else{
					printf("\nIdentificador do n� fora de range. Deve estar entre 0 e 63 inclusiv�\n");
				}
			}else{
				printf("\nMensagem mal formatada!\n");
			}
		}else{
			printf("\nMensagem incorrecta!\n");
		}
	}
	//deve ser para passar a ser n� de arranque
	else if(num_words==1){
		if(strcmp(message[0], "BOOT")==0){
			//muda a variavel que diz se somos n� de arranque ou nao
			transversal_data.serv_arranq = 1;
		}
		else{
			printf("\nMensagem incorrecta\n");
		}
	}
	//mensagem esta mal formatada
	else{
		printf("\nEst� tudo mal!\n");
	}

	return;
}
