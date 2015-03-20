typedef struct connect_fd {
  char id;
  int fd;
  struct connect_fd * next;
} connect_fd;

connect_fd * add_fd(connect_fd * new_fd, connect_fd * primeiro);
connect_fd * search_fd(char* id, connect_fd * primeiro, int * socket);
connect_fd * remove_fd(connect_fd * rmv_fd, connect_fd * primeiro);
