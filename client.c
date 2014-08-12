#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
/****************************
 * Function name:eroor
 * argument : message to print
 * task : give error msg at stdout
****************************/
void error(char *msg)
{
    perror(msg);
    exit(0);
}
/*********************************
 * Function name : main
 * argumrents : server machine address
 * 				& port on which server is 
 * 				running.
 * task : it is simple client programme 
 * 		 which Send msg to server and wait
 * 		 for responce. 
*********************************/
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
	struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    if (argc < 3) 
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    while(1)
   { 
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) 
			error("Error,opening socket");
		server = gethostbyname(argv[1]);
		if (server == NULL) 
		{
			fprintf(stderr,"Error, no such host\n");
			exit(0);
		}
		memset((char *)&serv_addr,NULL,sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
		serv_addr.sin_port = htons(portno);
		if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
			error("Error connecting");
		printf("Please enter the message: ");
		bzero(buffer,256);
		fgets(buffer,255,stdin);
		//printf("%s",buffer);
		n = write(sockfd,buffer,strlen(buffer));
		//printf("n=%d\n",n);
		if (n < 0) 
			 error("Error writing to socket");
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) 
			 error("Error reading from socket");
		printf("%s\n",buffer);
		close(sockfd);
	}	
    return 0;
}
