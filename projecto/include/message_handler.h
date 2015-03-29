/** \file message_handler.h
Cabeçalhos e protótipos associados às funções relacionadas com o processamento
das mensagens recebidas.
*/


#ifndef RCI_MESSAGEHANDLER
#define RCI_MESSAGEHANDLER




void preenche_pred_info(struct transversal_data * transversal_data, char* id,
	char* ip, char* porto, int new_fd);
	
void preenche_succ_info(struct transversal_data * transversal_data, char* id,
	char* ip, char* porto, int new_fd);

int dist(int ele, int eu);
int verifica_se_responsavel(char * c, int eu_id, int predi_id);

void handle_new_message(char* buffer,struct transversal_data * transversal_data,
	int new_fd);
int check_new_message(char** message, int num_words);

void handle_pred_message(char* buffer,struct transversal_data * transversal_data,
	int new_fd);
int check_pred_message(char** message, int num_words);

void handle_succ_message(char* buffer,struct transversal_data * transversal_data,
	int new_fd);
int check_succ_message(char** message, int num_words);

#endif
