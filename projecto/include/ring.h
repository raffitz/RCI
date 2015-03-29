

#ifndef RCI_RING
#define RCI_RING

#include "common.h"

int dist(int ele, int eu);
int verifica_se_responsavel(char * c, int eu_id, int predi_id);

void join_ring(char* ring, char* num,struct transversal_data *transversal_data);
void join_spec_ring(char* ring, char* num, char* succ, char* node,char* service,
struct transversal_data *transversal_data);

#endif
