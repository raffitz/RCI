//test.c
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char** argv)
{
                                                          
struct hostent *h;
struct in_addr *a;                                 
	if(argc==1){
		exit(0);
	}
	if((h=gethostbyname(argv[1]))==NULL){
		exit(1);//error  
	}
	printf("official host name: %s\n",h->h_name);        
	a=(struct in_addr*)h->h_addr_list[0];
	printf("internet address: %s (%08lX)\n",inet_ntoa(*a),(long unsigned int)ntohl(a->s_addr));
	exit(0);
}



