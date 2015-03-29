#include "fd_list.h"
#include <stdio.h>
#include <stdlib.h>

connect_fd * add_fd(connect_fd * new_fd, connect_fd * primeiro){
  connect_fd * aux;

  if (primeiro == NULL){
    primeiro = new_fd;
    new_fd->next=NULL;
  }else{
    aux=primeiro;
    while(aux->next!=NULL){
      aux=aux->next;
    }
    aux->next=new_fd;
    new_fd->next=NULL;
  }
  return primeiro;
}

connect_fd * search_fd(char* id, connect_fd * primeiro, int * socket){
	connect_fd * aux;

	aux=primeiro;

	if (primeiro == NULL){
		return NULL;
	}else{
		while(aux->id!=id[0]){
			aux=aux->next;
			if (aux==NULL){
				return NULL;
			}
		}
	}
	*socket = aux->fd;
	return aux;
}


connect_fd * remove_fd(connect_fd * rmv_fd, connect_fd * primeiro){
	connect_fd * aux = primeiro;
	if(primeiro == NULL || rmv_fd == NULL) return primeiro;
	if(rmv_fd!=primeiro){
		while((*aux).next != rmv_fd && (*aux).next != NULL){
			aux = (*aux).next;
		}
		if ((*aux).next == rmv_fd){
			(*aux).next = (*rmv_fd).next;
			free(rmv_fd);
		}
		
	}else{
		aux = primeiro;
		primeiro=rmv_fd->next;
		free(aux);
	}
	return primeiro;
}
