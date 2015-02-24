//test2.c
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char** argv)
{
	int fd, n;
	struct sockaddr_in addr;
	struct sockaddr_in addr2;
	size_t addrlen;
	struct hostent *h,*h2;
	struct in_addr *a;   
	char buffer[128];
	
	if(argc<2) exit(0);
	
	fd=socket(AF_INET,SOCK_DGRAM,0);//UDP socket
	if(fd==-1)exit(1);//error
	
	if((h=gethostbyname("tejo"))==NULL){
		exit(1);//error  
	}
	printf("official host name: %s\n",h->h_name);        
	a=(struct in_addr*)h->h_addr_list[0];
	
	
	memset((void*)&addr,(int)'\0',sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr=*a;
	addr.sin_port=htons(8000);
	//printf("trying to send <%s>\n",argv[1]);
	n=sendto(fd,argv[1],strlen(argv[1]),0,(struct sockaddr*)&addr,sizeof(addr));
	if(n==-1)exit(1);
	
	addrlen=sizeof(addr2);
	n=recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr2,&addrlen);
	if(n==-1)exit(1);//error
	
	write(1,"echo: ",6);//stdout
	write(1,buffer,n);
	write(1,"\n",1);
	h2 = gethostbyaddr(&addr2.sin_addr,sizeof(addr.sin_addr),AF_INET);
	if(h2 == NULL){
		printf("sent by [%s:%hu]\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
	}else{
		printf("sent by [%s:%hu]\n",h->h_name,ntohs(addr.sin_port));
	}
	close(fd);
	exit(0);
}
