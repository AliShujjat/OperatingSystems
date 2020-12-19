/* Mazeyar Moeini Feizabadi (mf04026)
   Mohammed Ali Shujjat (as03856) */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include "gp01_linkedlist.c"
#include <time.h>

//Maximum number of clients allowed
#define maxi 4

//struct about the client 
struct thread
{
	int sock;
	char name[256];		
};

//pthread
pthread_t tid;

//Out clients
int x;
int clients = 0;

//error function
void error(char *msg)
{
    perror(msg);
    exit(1);
}

void* ourServer(void *socker)
{	
	int n = 0;
	char* message[256] = {0};
	int sock = *((int *)socker);
	char * token = NULL;
	char buffer[256], name[256], space[2];
	char send[256];

	char lists[256];
	strcpy(space," ");
	 	
	bzero(name, 256);	
	read(sock, name, 255);
	
	//Maximum capacity of clients met
	if(clients == maxi)
	{
		write(sock ,"The Server is currently Full", 28);
		return 0;
	}
	
	//If quit function is called
	struct node *ptr = head;

	while(ptr != NULL)
	{
		if (strncmp(name, ptr->name, 256) == 0)
		{
			write(sock, "/quit", 5);
			return 0;
		}
		ptr = ptr->next;
	}

	write(sock, "Server is waiting", 17);

	//Storing identifiers and name of clients in linkedList
	insertFirst(sock, (char *)name);
	
	//Increasing no. of clients
	clients++;
	
	//Main Server Loop
	while(strncmp(buffer, "/quit", 5))	
	{	
		bzero(buffer, 255);	
		n = read(sock, buffer, 255);	

		if(n < 0) printf("Error reading from socket %d\n",x);
		
		//Command for list entered	
		if(strncmp(buffer, "/list", 5) == 0)
		{
			//Printing list on server side
			printList();

			struct node *ptr = head;

			memset(lists,0,255);

			strcat(lists,"\nClients on server:\n");

			while(ptr != NULL)
			{
				strcat(lists,ptr->name);
				strcat(lists,"\n");
				
				ptr = ptr->next;
			}

			write(sock, lists, strlen(lists));

		}
		
		//Command for message entered
		if (strncmp(buffer, "/msg", 4) == 0)
		{	//Our local variables
			int var = 0;
			int flag = 0;
			memset(message, 0, 255);
			memset(send,0,255);
			buffer[n] = ' ';
			
			printf("Sending Message: %s \n", buffer);			
			token = strtok (buffer," ");
			
			//splits on space
			while (token != NULL)
			{	  			
  				message[var] = token;
  				token = strtok (NULL, " ");
				var++;
  			}

			message[var] = '\0';

			strcat(send,message[1]);
			strcat(send," -> ");

			for(int i = 2; i<var; i++){
				strcat(send,message[i]);
				strcat(send," ");
			}
			
			//Checking validity of client name
			if(message[1] != 0)
			{
				struct node *ptr = head;

				while(ptr != NULL)
				{
					if(message[1] == 0)
					{
						break;
					}			 

					else if(message[1] != 0 && strncmp(message[1],ptr->name,strlen(message[1])) == 0)
					{
						flag = 1;
						break;		
					}

					ptr = ptr->next;
				}
			} 		
			
			//Invalid client name
			if(flag == 0)
			{
				write(sock, "\nThe client name you've provided is incorrect or missing", 56);
			}

			//Message sending successful
			else
			{	int identity;
				char var[256];

				struct node *ptr = head;

				while(ptr != NULL)
				{
					if (strncmp(message[1], ptr->name, strlen(message[1])) == 0)
					{
						identity = ptr->key;
					}

					if(sock == ptr->key)
					{
						strcpy(var, ptr->name);
					}

					ptr = ptr->next;	
				}

				write(identity,send,strlen(send)+1);
				bzero(send, 256);
			}	
		}

		if( n < 0 )printf("Error writing to socket %d \n",x);
	}


	//removes client from linkedList when quitted
	delete(sock);

	//Subtract from number of active clients
	clients--;
}

//OUR MAIN
int main(int argc, char *argv[])
{	

     int sockfd, newsockfd, portno, clilen,err;
     struct sockaddr_in serv_addr, cli_addr;
     void *socker;

     if (argc < 2) 
     {
         fprintf(stderr,"ERROR, port not provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0) error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);

     //Prepares the sockaddr_in 
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     //Bind	
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
     //Listening at port
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     //Main loop    
     while(1)
	{
     		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     		socker = &newsockfd;
    		if (newsockfd < 0) 
         		error("ERROR on accept");
		err = pthread_create(&(tid), NULL, ourServer, socker);
       		if (err != 0)
            		error("Failed");
        	else
            	{	
            		printf("\nClient connected successfully\n");
				}	
    };

	 close(sockfd);
   	 close(newsockfd);
     return 0; 
}
