/** \file net_udp.h
Cabeçalhos e protótipos associados às funções relacionadas com o protocolo UDP.
*/

#ifndef RCI_NETUDP
#define RCI_NETUDP


void getudpdest(char*node,char*service,struct startup_data *startup_data);

void createsocket_udp(struct transversal_data *transversal_data);

void probe_my_IP(struct transversal_data *transversal_data);

#endif
