/** \file options.c
Ficheiro responsável pela interpretação dos argumentos passados via linha de
comandos. As funções implementadas neste módulo correspondem à interpretação dos
argumentos passados via linha de comandos, e correspondente transferência para a
estrutura de dados transversal aos vários módulos do projecto.
*/

#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "options.h"
#include "net_udp.h"

/**
Função que mostra a mensagem Usage.
*/
void usage(char* appname){
	printf("Usage: %s %s %s %s\n",appname,"[-t <ringport>]","[-i bootIP]",
		"[-p bootport]");
	exit(0);
}

/**
Função responsável pela interpretação dos argumentos passados via linha de
comandos.
Esta função recorre à função getopt() de maneira a obter os diversos parâmetros
passados via linha de comandos. Os argumentos são passados via ponteiro para
estrutura de maneira a que a função seja compatível com threads.
*/
void* parse_options(struct args_parse_options *params){
	int i;
	unsigned char gt,gi,gp;
	char hostname[256];
	char port[16];
	
	extern char *optarg;
	extern int opterr;
	
	opterr=0;
	
	gt = gi = gp = 0;
	/* Leitura dos parâmetros: */
	while(1){
		i = getopt((*(*params).argc),(*(*params).argv),"t:i:p:");
		if (i=='?'){
			usage((*(*params).argv)[0]);
		}
		if (i==-1){
			break;
		}
		switch(i){
			case 't':
				gt++;
				strncpy((*(*params).startup_data).ringport,
					optarg,16);
				break;
			case 'i':
				gi++;
				strncpy(hostname,optarg,256);
				break;
			case 'p':
				gp++;
				strncpy(port,optarg,16);
				break;
			default:
				break;
		}
	}
	
	/* Se algum dos três parâmetros não foi passado, o programa sai. */
	if(gt==0 || gi==0 || gp==0){
		usage((*(*params).argv)[0]);
	}
	
	/* Caso contrário, é determinada a estrutura de destino: */
	getudpdest(hostname,port,(*params).startup_data);
	
	
	
	return NULL;
}
