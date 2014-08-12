/*************************************
 * Programme for server
 * purpose : get message from client and
			 Ack client.
*************************************/
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void dostuff(int);  // function prototype
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, pid;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) 
     {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
         error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
	while (1) 
	{
         newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
            dostuff(newsockfd);
            exit(0);
         }
         else close(newsockfd);
     } 
    return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 Function name: dostuff()
 Work : This Function manages all communication 
		once connection has been established.
*****************************************/
void dostuff (int sock)
{
   int n;
   char buffer[256];
   char cmd[256];   
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) 
   error("ERROR reading from socket");
   printf("Message from client: %s\n",buffer);
   sprintf(cmd,"wall %s",buffer);
   system(cmd);
   n = write(sock,"ACK for Client",15);
   if (n < 0) 
   error("ERROR writing to socket");
}
