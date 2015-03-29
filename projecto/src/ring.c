
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ring.h"
#include "common.h"
#include "message_handler.h"
#include "net_tcp.h"
#include "net_common.h"

#define RCI_MSGSIZE 256

void join_ring(char* ring, char* num, struct transversal_data *transversal_data)
{
	char buffer[RCI_MSGSIZE];
	char message[6][RCI_MSGSIZE];
	
	int ml;
	
	int bufsize;
	
	int fd;
	
	sprintf(buffer,"BQRY %s",ring);
	bufsize = strlen(buffer);

#ifdef RCIDEBUG1
	printf("RCIDEBUG1: sending message: <%s>\n",buffer);
#endif
	sendto((*transversal_data).u,buffer,bufsize,0,
		(*transversal_data).startup_data.destination,
		(*transversal_data).startup_data.dest_size);


	buffer[recvfrom((*transversal_data).u,buffer,256,0,NULL,NULL)] = '\0';
	
	ml = sscanf(buffer,"%s %s %s %s %s %s",message[0],message[1],message[2],
		message[3],message[4],message[5]);
	
#ifdef RCIDEBUG1
	printf("RCIDEBUG1: %s\n",buffer);
#endif
	if((ml == 1) && strcmp(message[0], "EMPTY")==0){
	//Regista-se no servidor de arranque e se receber OK assume-se como no de arranque do novo anel
		sprintf(buffer, "REG %s %s %s %s", ring, num, transversal_data->ext_addr, transversal_data->startup_data.ringport);
		sendto((*transversal_data).u, buffer,strlen(buffer)-1,0, (*transversal_data).startup_data.destination,
			(*transversal_data).startup_data.dest_size);
		buffer[recvfrom((*transversal_data).u,buffer,256,0,NULL,NULL)] = '\0';
		if(strcmp(buffer, "OK")==0){
			sscanf(ring,"%d",&(transversal_data->ring));
			sscanf(num,"%d",&(transversal_data->id));
			transversal_data->serv_arranq = 1;
		}else{
			printf("Problema no servidor de arranque!\n");
			return;
		}
	}else if((ml == 5) && strcmp(message[0],"BRSP")==0){
		//Recebe a informacao do no de arranque do anel e liga-se
		if(strcmp(message[2],num)==0){
			printf("Esse id já existe no anel. Escolha outro.\n");
			return;
		}
		sprintf(buffer,"ID %s",num);
		
		fd = connect_tcp(message[3],message[4]);
		
		write_message(buffer,fd);
		
		read_message_tcp(buffer,RCI_MSGSIZE,fd);
		
		ml = sscanf(buffer,"%s %s %s %s %s %s",message[0],message[1],
			message[2],message[3],message[4],message[5]);
		
		if(ml!=4 || strcmp(message[0],"SUCC")!= 0){
#ifdef RCIDEBUG1
			printf("ID recebeu resposta má.\n");
#endif
			return;
		}
		
		if(strcmp(message[1],num)==0){
			printf("Esse id já existe no anel. Escolha outro.\n");
			return;
		}
		
		sprintf(buffer,"NEW %s %s %s",num, transversal_data->ext_addr,
			transversal_data->startup_data.ringport);
		
		fd = connect_tcp(message[2],message[3]);
		
		write_message(buffer,fd);
		
		preenche_succ_info(transversal_data, message[1],message[2],
			message[3],fd);
			
		sscanf(num,"%d",&(transversal_data->id));
		
	}else{
#ifdef RCIDEBUG1
		printf("Mensagem mal formatada!\n");
#endif
	}

	return;
}
