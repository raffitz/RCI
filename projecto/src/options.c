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
#include "common.h"
#include "options.h"

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
comandos. Esta função recorre à função getopt() de maneira a obter os diversos
parâmetros passados via linha de comandos. Os argumentos são passados via
ponteiro para estrutura de maneira a que a função seja compatível com threads.
*/
void* parse_options(struct args_parse_options *params){
	int i;
	char gt,gi,gp;
	
	extern char *optarg;
	extern int optind,opterr,optopt;
	
	opterr=0;
	
	gt = gi = gp = 0;
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
				break;
			case 'i':
				gi++;
				break;
			case 'p':
				gp++;
				break;
			default:
				break;
		}
	}
	
	if(gt==0 || gi==0 || gp==0){
		usage((*(*params).argv)[0]);
	}
	
	return NULL;
}
