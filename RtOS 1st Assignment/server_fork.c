/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   
   Rensselaer Polytechnic Institute (RPI)
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void  connection_handler(int *socket_desc);

void error(char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  if (argc < 2) {
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
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
           sizeof(serv_addr)) < 0) 
    error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  
  
  while (1)
    {	
		pid_t pid;
    	if((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen))<0){
			error("Accept failed");
    		exit(1);
    	}
    	puts("New Connection Accepted");
    	
    	if((pid = fork()) <0 ){
		error("fork failed");
		exit(1);
		}
		if(pid == 0){
			connection_handler( &newsockfd );
			exit(1);
		}  
    }
}

void connection_handler(int *socket_desc)
{
    int new_socket = *socket_desc;
    char client_buffer[255];
    int flag = read(new_socket,client_buffer,255);
     
    
    if(flag > 0){
	    printf("Here is the message: %s",client_buffer);
	    write(new_socket,"Message received",22);
 	}
    else if(flag == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else
    {
        error("Couldn't Receive");
    }
    close(new_socket);     
} 
