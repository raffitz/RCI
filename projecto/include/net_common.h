/** \file net_common.h
Cabeçalhos e protótipos associados às funções relacionadas com a rede mas não
associadas a nenhum protocolo em específico.
*/

#ifndef RCI_NETCOMMON
#define RCI_NETCOMMON

void getsockaddr(char *node,char *service,int *family,socklen_t *size,
	struct sockaddr **sockaddress,int protocol);
	
void getIP(struct sockaddr *me, socklen_t mysize,char*node,char*service);

void write_message(char * buffer, int fd);

#endif
