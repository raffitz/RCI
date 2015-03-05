/** \file common.h
Cabeçalho com declarações das estruturas de dados transversais aos vários
módulos do projecto.
*/

#ifndef RCI_COMMON
#define RCI_COMMON

/**
Estrutura com os dados a ser recolhidos pela linha de comandos.
*/
struct startup_data{
	/** Porto do servidor TCP */
	int ringport;
	/** Endereço do servidor de arranque */
	unsigned char boot_ip[4];
	/** Porto do servidor de arranque */
	int bootport;
};

/**
Estrutura com as referências necessárias aos vários módulos do projecto.
*/
struct transversal_data{
	/** socket UDP */
	int u;
	/** socket TCP */
	int t;
	/** Anel a ser utilizado */
	int ring;
	/** Parâmetros da linha de comandos */
	struct startup_data params;
};

#endif
