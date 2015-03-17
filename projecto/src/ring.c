
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ring.h"
#include "common.h"

#define RCI_MSGSIZE 256

int join_anel(char* ring, char* num, struct transversal_data *transversal_data)
{
	char buffer[RCI_MSGSIZE];
	int bufsize;
	int rcvsize;
	sprintf(buffer,"BQRY %s",ring);
	bufsize = strlen(buffer) - 1;
	
	sendto((*transversal_data).u,buffer,bufsize,0,(*transversal_data).startup_data.destination,(*transversal_data).startup_data.dest_size);
	
	rcvsize = recvfrom((*transversal_data).u,buffer,bufsize,0,NULL,NULL);
	buffer[rcvsize] = 0;
	printf("%s\n",buffer);
	
	return 0;
}
