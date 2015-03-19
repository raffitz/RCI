

int check_message(char** message, int num_words);
void write_message_tcp(char * string, int socket);
void trata_messagem(char* buffer);
void preenche_predi_info(struct transversal_data * transversal_data, char* id, char* ip, char* porto);
int dist(int ele, int eu);
int verifica_se_responsavel(char * c, int eu_id, int predi_id);
