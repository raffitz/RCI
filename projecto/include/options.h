/** \file options.h
Cabeçalhos e protótipos associados ao módulo de processamento das
opções de linha de comandos.
*/

#ifndef RCI_OPTIONS
#define RCI_OPTIONS

#include "common.h"



void* parse_options(int* argc, char*** argv,
	struct transversal_data *transversal_data);



#endif
