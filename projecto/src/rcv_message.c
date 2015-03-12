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
				printf("\nMensagem mal formatada\n");
			}
		}else{
			printf("\nMensagem incorrecta\n");
		}
	}
	//Deve ser search
	else if(num_words==2){
		int no_proc_id;

		if(strcmp(message[0], "search")==0){
			if(sscanf(message[1], "%d", &no_proc_id)==1){
				if(no_proc_id>=0 && no_proc_id<64){
				//faz search

				}else{
					printf("\nIdentificador do nó fora de range. Deve estar entre 0 e 63 inclusivé\n");
				}
			}else{
				printf("\nComando mal formatado. Deverá ser da forma:\n\t[search k] -> O utilizador pretende saber o identificador e a localização do nó responsável pelo identificador k.\n");
			}
		}else{
			printf("\nEscolha um dos comandos acima!\n");
		}
	}
	//deve ser leave ou show ou exit
	else if(num_words==1){
		if(strcmp(message[0], "BOOT")==0){
			//muda a variavel que diz se somos no de arranque ou nao
		}
		else{
			printf("\nMensagem incorrecta\n");
		}
	}
	//mensagem esta mal formatada
	else{
		printf("\nEscolha um dos comandos acima!\n");
	}

	return
}
