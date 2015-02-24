//test.c
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char** argv)
{
	int fd, n, nbytes, nleft, nwritten, nread;
	char *ptr, buffer[128];
	struct sockaddr_in addr;
	struct hostent *h;
	struct in_addr *a;
	
	if(argc<2){
		exit(0);
	}
	
	fd=socket(AF_INET,SOCK_STREAM,0);//TCP socket
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
	n=connect(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(n==-1)exit(1);//error
	ptr=strcpy(buffer,argv[1]);
	nbytes=strlen(argv[1]);
	nleft=nbytes;
	while(nleft>0){nwritten=write(fd,ptr,nleft);
               if(nwritten<=0)exit(1);//error
		       nleft-=nwritten;
		       ptr+=nwritten;}
	nleft=nbytes; ptr=buffer;
	while(nleft>0){nread=read(fd,ptr,nleft);
               if(nread==-1)exit(1);//error
               else if(nread==0)break;//closed by peer
		       nleft-=nread;
		       ptr+=nread;}
	nread=nbytes-nleft;
	close(fd);
	write(1,"echo: ",6);//stdout
	write(1,buffer,nread);
	write(1,"\n",1);
	exit(0);
}
