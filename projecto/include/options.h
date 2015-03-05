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
	int* argc;
	char*** argv;
	struct startup_data *startup_data;
};



void* parse_options(struct args_parse_options *params);

#endif
