/** \file main.c
Ficheiro principal do projecto. Este ficheiro serve como chave de abóbada
para os vários módulos desenvolvidos e responsáveis pelas diversas
funcionalidades do projecto.
*/


#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "options.h"

/** Função principal do programa ddt. Esta função serve-se dos módulos criados
nos restantes ficheiros-fonte para implementar as funcionalidades necessárias ao
projecto, como especificadas no enunciado.
*/
int main(int argc, char**argv){
	
	struct transversal_data transversal_data;
	struct args_parse_options args_parse_options;
	
	/* Leitura das opções: */
	args_parse_options.startup_data = &(transversal_data.startup_data);
	args_parse_options.argc = &argc;
	args_parse_options.argv = &argv;
	parse_options(&args_parse_options);
	
	printf("Testing\n");
	exit(0);
}
