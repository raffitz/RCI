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
    new_fd->next=NULL
  }
  return primeiro;
}

connect_fd * search_fd(char* id, connect_fd * primeiro, int * socket){
  connect_fd * aux1, *aux2;

  aux1=primeiro;
  aux2=primeiro;

  if (primeiro == NULL){
    return NULL;
  }else{
    while(aux1->id!=id[0]){
      aux2=aux1;
      aux1=aux1->next;
      if (aux1==NULL){
        return NULL;
      }
    }
  }
  *socket = aux1->fd;
  return aux2;
}


connect_fd * remove_fd(connect_fd * rmv_fd, connect_fd * primeiro){
  connect_fd * aux;
  if(rmv_fd!=primeiro){
    aux=rmv_fd->next->next;
    free(rmv_fd->next);
    rmv_fd->next=aux;
  }else{
    primeiro=rmv_fd->next;
    free(rmv_fd->next);
  }
  return primeiro;
}
