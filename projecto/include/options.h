/** \file options.h
Cabeçalhos e protótipos associados ao módulo de processamento das
opções de linha de comandos.
*/

#ifndef RCI_OPTIONS
#define RCI_OPTIONS

#include "common.h"

/**
Estrutura com os argumentos para a função \ref parse_options.
*/
struct args_parse_options{
	/** Ponteiro para o local em memória do contador de argumentos. */
	int* argc;
	/** Ponteiro para o local em memória dos argumentos. */
	char*** argv;
	/** Ponteiro para uma das estruturas transversais ao projecto. */
	struct startup_data *startup_data;
};



void* parse_options(struct args_parse_options *params);



#endif
