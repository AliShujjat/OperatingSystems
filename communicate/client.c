#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <pthread.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

//Printing the error
void error(char *errorMessage)
{
    perror(errorMessage);
    exit(0);
}

//Reciever
void *reciever(void * sockfd)
{
    //Our Variables
	char buffer[256];
	int sock = *(int*) sockfd;
	int socketStatus;
	socketStatus = read(sock,buffer,256);

	while(!(socketStatus < 0))
    {
        if (socketStatus == 0)
        {
            printf("\nServer Unresponsive! Check server status\n");
            exit(0);
        }

		printf("\nReceived: ");
   		printf("%s",buffer);

		if(strncmp(buffer, "/quit", 5) == 0)
		{	
            printf("A client with this name already exists, please choose another name and try again.\n");
			exit(0);
		}

		if(strncmp(buffer, "The server is currently full", 28) == 0)
		{
			exit(0);
		}

		bzero(buffer,256);
		socketStatus = read(sock,buffer,256);
	}

	if (socketStatus < 0)
           error("Socket has been  cleared");
	return 0;
}

//Sender
void *sent(void *sockfd){
	char buffer[256];
	int sock = *(int*) sockfd;
	int socketStatus;

	while(strncmp(buffer, "/quit", 5))
    {
		bzero(buffer, 256);
		printf("\nEnter command: ");
		fgets(buffer, 256, stdin);
		socketStatus = write(sock,buffer,strlen(buffer));
	}

	printf("Client Closed");
	exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    char name[100];

    if (argc < 4) 
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    
    //Establishing Connection	
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
  
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    in_addr_t t = inet_addr(argv[1]);
    bcopy((char *) &t, 
         (char *)&serv_addr.sin_addr.s_addr,
         sizeof(in_addr_t));
   	serv_addr.sin_port = htons(portno);
  

    //Faliure
    int connectStatus = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if (connectStatus < 0)
    {
        error("Error1");
    }

    strcpy(name,argv[3]);
    send(sockfd,name,strlen(name),0);
   

    //Threads
    pthread_t tid, pid;
    pthread_create( &pid , NULL ,  sent , (void*) &sockfd);	
    pthread_create( &tid , NULL ,  reciever , (void*) &sockfd);

    pthread_join( pid , NULL);
    pthread_join( tid , NULL);
   
    return 0;
}


