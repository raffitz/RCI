/** \file common.c
Implementação das funções de gestão das estruturas transversais.
*/

#include <stdlib.h>
#include "common.h"


/** Função que (re)inicializa a estrutura transversal.
*/
void reset_td(struct transversal_data * transversal_data)
{
	(*transversal_data).u = -1;
	(*transversal_data).t = -1;

	(*transversal_data).peer_pred.id = -1;
	(*transversal_data).peer_pred.node[0] = 0;
	(*transversal_data).peer_pred.service[0] = 0;
	(*transversal_data).peer_pred.socket = -1;

	(*transversal_data).peer_succ.id = -1;
	(*transversal_data).peer_succ.node[0] = 0;
	(*transversal_data).peer_succ.service[0] = 0;
	(*transversal_data).peer_succ.socket = -1;

	(*transversal_data).ring = -1;
	(*transversal_data).id = -1;
	(*transversal_data).serv_arranq = 0;
	(*transversal_data).primeiro = NULL;

}
