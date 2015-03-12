#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void recv_message(char* buffer){
	char message[6][256];
	int num_words;
	num_words = sscanf(buffer, "%s %s %s %s %s %s", message[0],message[1],message[2],message[3],message[4],message[5]);

	//deve ser RSP
	if(num_words==6){
		int ini_id, procu_id, resp_id;
		if(strcmp(message[0], "RSP")==0){
			if(sscanf(message[1], "%d", &ini_id)==1 && sscanf(message[2], "%d", &procu_id)==1 && sscanf(message[3], "%d", &resp_id)==1){
				if((ini_id>=0 && ini_id<64) || (procu_id>=0 && procu_id<64) || (resp_id>=0 && resp_id<64)){
				//se ini_id for o mesmo que o present no entao responde. Se nao passa a mensagem.

				}else{
					printf("\nIdentificador do nó fora de range. Deve estar entre 0 e 63 inclusivé\n");
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
					printf("\nIdentificador do nó fora de range. Deve estar entre 0 e 63 inclusivé\n");
				}
			}else{
				printf("\nMensagem mal formatada!\n");
			}
		}
		else if(strcmp(message[0], "CON")==0){
			int new_succ;
			if(sscanf(message[1], "%d", &new_succ)==1){
				if(new_succ && new_succ<64){
					//tenho que me conectar com o nó i
				}else{
					printf("\nIdentificador do nó fora de range. Deve estar entre 0 e 63 inclusivé\n");
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
					printf("\nIdentificador do nó fora de range. Deve estar entre 0 e 63 inclusivé\n");
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
				//Vai ver se ele é responsavel pelo no. Depois responde adequadamente.

				}else{
					printf("\nIdentificador do nó fora de range. Deve estar entre 0 e 63 inclusivé\n");
				}
			}else{
				printf("\nMensagem mal formatada!\n");
			}
		}else{
			printf("\nMensagem incorrecta!\n");
		}
	}
	//Deve ser tentativa de um nó se juntar ao anel
	else if(num_words==2){
		int procu_id;

		if(strcmp(message[0], "ID")==0){
			if(sscanf(message[1], "%d", &procu_id)==1){
				if(procu_id && procu_id){
				//faz search do no que se procura enviando QRY j i ao succi

				}else{
					printf("\nIdentificador do nó fora de range. Deve estar entre 0 e 63 inclusivé\n");
				}
			}else{
				printf("\nMensagem mal formatada!\n");
			}
		}else{
			printf("\nMensagem incorrecta!\n");
		}
	}
	//deve ser para passar a ser nó de arranque
	else if(num_words==1){
		if(strcmp(message[0], "BOOT")==0){
			//muda a variavel que diz se somos nó de arranque ou nao
		}
		else{
			printf("\nMensagem incorrecta\n");
		}
	}
	//mensagem esta mal formatada
	else{
		printf("\nEstá tudo mal!\n");
	}

	return;
}
