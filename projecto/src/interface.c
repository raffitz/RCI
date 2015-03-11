#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int interface(){
	int num_com=0;
	char comands[6][256];
	char str[256];
	
	printf("Escolha uma das instruções para executar:\n\n\t[join x i] -> O utilizador pretende que o nó se junte ao anel x tomando i como identificador nesse anel. O nó só pode pertencer a um anel de cada vez.\n\n\t[join x i succi succi.IP succi.TCP] -> O utilizador pretende que o nó se junte ao anel x tomando i como identificador nesse anel e succi como o identificador do seu sucessor, localizado em (succi.IP succi.TCP).\n\n\t[leave] -> O utilizador pretende que o nó abandone o anel a que pertence.\n\n\t[show] -> Mostra ao utilizador o número do anel, o identificador do nó nesse anel, bem como os identificadores do seu sucessor e do seu predecessor.\n\n\t[search k] -> O utilizador pretende saber o identificador e a localização do nó responsável pelo identificador k.\n\n\t[exit] -> O utilizador fecha a aplicação.\n");

	while(1){
		printf("\n");	
		fgets(str, 256, stdin);
		num_com=sscanf(str, "%s %s %s %s %s %s", comands[0], comands[1], comands[2], comands[3], comands[4], comands[5]);
		
		/*for(int i=0; i<num_com; i++){
			printf("%s\n", comands[i]);
		}*/	
		
		//deve ser join succi
		if(num_com==6){
		//faz join succi
		}
		//deve ser join
		else if(num_com==3){
		//faz join 
	
		}
		//Deve ser search
		else if(num_com==2){
			int no_proc_id;
			
			if(strcmp(comands[0], "search")==0){
				if(sscanf(comands[1], "%d", &no_proc_id)==1){
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
		else if(num_com==1){
			if(strcmp(comands[0], "leave")==0){
			//faz leave
		
			}else if(strcmp(comands[0], "show")==0){
			//faz show
		
			}else if(strcmp(comands[0], "exit")==0){
			//faz exit	
				return (0);
			}else{
				printf("\nEscolha um dos comandos acima!\n");
			}
		}
		//mensagem esta mal formatada
		else{
			printf("\nEscolha um dos comandos acima!\n");
		}
	}	
}