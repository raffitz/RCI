/** \file message_handler.h
Cabeçalhos e protótipos associados às funções relacionadas com o processamento
das mensagens recebidas.
*/


#ifndef RCI_MESSAGEHANDLER
#define RCI_MESSAGEHANDLER


int check_message(char** message, int num_words);
void trata_mensagem(char* buffer, struct transversal_data * transversal_data, int new_fd);
void preenche_predi_info(struct transversal_data * transversal_data, char* id,
	char* ip, char* porto, int new_fd);
int dist(int ele, int eu);
int verifica_se_responsavel(char * c, int eu_id, int predi_id);

void handle_new_message(char* buffer,struct transversal_data * transversal_data,
int new_fd);

#endif
