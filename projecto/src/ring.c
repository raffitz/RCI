
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ring.h"
#include "common.h"
#include "message_handler.h"

#define RCI_MSGSIZE 256

int join_ring(char* ring, char* num, struct transversal_data *transversal_data)
{
	char buffer[RCI_MSGSIZE];
	int bufsize;
	sprintf(buffer,"BQRY %s",ring);
	bufsize = strlen(buffer);

#ifdef RCIDEBUG1
	printf("RCIDEBUG1: sending message: <%s>\n",buffer);
#endif
	sendto((*transversal_data).u,buffer,bufsize,0,
		(*transversal_data).startup_data.destination,
		(*transversal_data).startup_data.dest_size);


	buffer[recvfrom((*transversal_data).u,buffer,256,0,NULL,NULL)] = '\0';

#ifdef RCIDEBUG1
	printf("RCIDEBUG1: %s\n",buffer);
#endif
	if(strcmp(buffer, "EMPTY")==0){
	//Regista-se no servidor de arranque e se receber OK assume-se como no de arranque do novo anel
		sprintf(buffer, "REG %s %s %s %s", ring, num, transversal_data->ext_addr, transversal_data->startup_data.ringport);
		sendto((*transversal_data).u, buffer,strlen(buffer)-1,0, (*transversal_data).startup_data.destination,
			(*transversal_data).startup_data.dest_size);
		buffer[recvfrom((*transversal_data).u,buffer,256,0,NULL,NULL)] = '\0';
		if(strcmp(buffer, "OK")==0){
			sscanf(ring,"%d",&(transversal_data->ring));
			sscanf(num,"%d",&(transversal_data->id));
			transversal_data->serv_arranq = 1;
			//Por o fd do accept no select
			FD_SET(transversal_data->t, &transversal_data->fds);
		}else{
			printf("Problema no servidor de arranque!\n");
		}
	}else{
	//Recebe a informacao do no de arranque do anel e liga-se
		//Por o fd do accept no select
		FD_SET(transversal_data->t, &transversal_data->fds);
		sscanf(num,"%d",&(transversal_data->id));
		trata_mensagem(buffer, transversal_data, -1);
	}

	return 0;
}
