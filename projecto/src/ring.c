
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include "ring.h"
#include "common.h"
#include "message_handler.h"
#include "net_tcp.h"
#include "net_common.h"

#define RCI_MSGSIZE 256


#define RCI_MAXNR 64

/** Função de cálculo da distância entre dois nós. */
int dist(int ele, int eu){
	if (eu>=ele){
		return (eu-ele);
	}else{
		return(RCI_MAXNR+eu-ele);
	}

}

/** Função de verificação da responsabilidade de um nó.
	Valores retornados: 1-> responsavel / 0->nao responsavel */
int verifica_se_responsavel(char * c, int eu_id, int pred_id){

	int id;
	if(eu_id == pred_id) return 1;
	if(pred_id>=0){
		sscanf(c, "%d", &id);
		if(dist(id, eu_id)<dist(id, pred_id)){
			return 1;
		}else{
			return 0;
		}
	}else{
		return 1;
	}
}


void join_ring(char* ring, char* num, struct transversal_data *transversal_data)
{
	char buffer[RCI_MSGSIZE];
	char message[6][RCI_MSGSIZE];
	
	int ml;
	
	int bufsize;
	
	char counter;
	
	int fd;
	
	fd_set to_fds;
	
	struct timeval timeout;
	
	
	


	

	
	for(counter=0;counter<5;counter++){
		FD_ZERO(&to_fds);
		FD_SET((*transversal_data).u,&to_fds);
		
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
		
		sprintf(buffer,"BQRY %s",ring);
		bufsize = strlen(buffer);
		sendto((*transversal_data).u,buffer,bufsize,0,
			(*transversal_data).startup_data.destination,
			(*transversal_data).startup_data.dest_size);
		
#ifdef RCIDEBUG1
		printf("RCIDEBUG1: sending message: <%s>\n",buffer);
#endif
		
		
		if(select((*transversal_data).u+1,&to_fds,NULL,NULL,
			&timeout)<1) continue;
	
		if(FD_ISSET((*transversal_data).u,&to_fds)){
			buffer[recvfrom((*transversal_data).u,buffer,256,0,NULL,
				NULL)] = '\0';
			break;
		}
		printf("Timeout elapsed. No contact from server.\n");
		return;
	}
	
	
	
	ml = sscanf(buffer,"%s %s %s %s %s %s",message[0],message[1],message[2],
		message[3],message[4],message[5]);
	
#ifdef RCIDEBUG1
	printf("RCIDEBUG1: %s\n",buffer);
#endif
	if((ml == 1) && strcmp(message[0], "EMPTY")==0){
	//Regista-se no servidor de arranque e se receber OK assume-se como no de arranque do novo anel
	
	
		for(counter=0;counter<5;counter++){
			FD_ZERO(&to_fds);
			FD_SET((*transversal_data).u,&to_fds);
		
			timeout.tv_sec = 3;
			timeout.tv_usec = 0;
		
			sprintf(buffer, "REG %s %s %s %s", ring, num, transversal_data->ext_addr, transversal_data->startup_data.ringport);
			bufsize = strlen(buffer);
			sendto((*transversal_data).u,buffer,bufsize,0,
				(*transversal_data).startup_data.destination,
				(*transversal_data).startup_data.dest_size);
		
			if(select((*transversal_data).u+1,&to_fds,NULL,NULL,
				&timeout)<1) continue;
	
			if(FD_ISSET((*transversal_data).u,&to_fds)){
				buffer[recvfrom((*transversal_data).u,buffer,
					256,0,NULL,NULL)] = '\0';
				break;
			}
			printf("Timeout elapsed. No contact from server.\n");
			return;
		}
		
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
		sprintf(buffer,"ID %s\n",num);

#ifdef RCIDEBUG1
		printf("Attempting to send <%s:%s>:%s\n",message[3],message[4],
			buffer);
#endif
		
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
		
		sprintf(buffer,"NEW %s %s %s\n",num, transversal_data->ext_addr,
			transversal_data->startup_data.ringport);
			
#ifdef RCIDEBUG1
		printf("Attempting to send <%s:%s>:%s\n",message[2],message[3],
			buffer);
#endif
		
		fd = connect_tcp(message[2],message[3]);
		
		write_message(buffer,fd);
		
		preenche_succ_info(transversal_data, message[1],message[2],
			message[3],fd);
			
		sscanf(num,"%d",&(transversal_data->id));
		sscanf(ring,"%d",&(transversal_data->ring));
		
	}else{
#ifdef RCIDEBUG1
		printf("Mensagem mal formatada!\n");
#endif
	}

	return;
}

void join_spec_ring(char* ring, char* num, char* succ, char* node,char* service,
struct transversal_data *transversal_data)
{
	char buffer[RCI_MSGSIZE];
	
	int fd;
	
	
	
	sprintf(buffer,"NEW %s %s %s\n",num, transversal_data->ext_addr,
			transversal_data->startup_data.ringport);
			
#ifdef RCIDEBUG1
	printf("Attempting to send <%s:%s>:%s\n",node,service,buffer);
#endif
		
	fd = connect_tcp(node,service);
	
	write_message(buffer,fd);
	
	preenche_succ_info(transversal_data,succ,node,service,fd);
		
	sscanf(num,"%d",&(transversal_data->id));
	sscanf(ring,"%d",&(transversal_data->ring));

	return;
}
