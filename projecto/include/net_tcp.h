/** \file net_tcp.h
Cabeçalhos e protótipos associados às funções relacionadas com o protocolo TCP.
*/

#ifndef RCI_NETTCP
#define RCI_NETTCP

void createserver_tcp(struct transversal_data *transversal_data);
int connect_tcp(char* node, char* service);
int read_message_tcp(char* dest,int destsize, int fd);

#endif
