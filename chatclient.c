/***************************************************************
Name: Lauren Boone
Date: 1/28/2020
Description: This is a chat client that communicates back and forth
with a server. The client initiates the chat. 
Sources: beejs guide, 344 OTP project(most of the code is from that)
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues


/***************************************************
startConnection()
Description: This function takes the serverName 
and port as parameters. It calls the getaddrinfo
to identify a host a service. A addrinfo struct 
is returned. 
//https://beej.us/guide/bgnet/html/#socket
******************************************************/
struct addrinfo* startConnection(char servName[], char *port) {
	//This is taken mostly from beej's guide 
	//printf("getting address info");
	struct addrinfo *res;
	struct addrinfo hints;
	int socketfd;//used to see return value
	memset(&hints, 0, sizeof hints);//clear memory
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM; //use TCP
	
	//source https://beej.us/guide/bgnet/html/#getaddrinfoprepare-to-launch
	
	socketfd = getaddrinfo(servName, port, &hints, &res);//get the address info
	if (socketfd != 0) {//exit if error
		error("Please enter correct port");
	}

	return res;//return the struct
	
}


/**********************************************************
getSocket
This function tries to create a socket useing the serve name
The socket file descripter is returned
************************************************************/
int getSocket(struct addrinfo *sockName) {
	int ret;
	//create the socket and assign the file descriptor value to ret
	//https://beej.us/guide/bgnet/html/#socket
	if ((ret = socket(sockName->ai_family, sockName->ai_socktype, sockName->ai_protocol)) == -1) {
		error("Error in creating socket");
	}
	return ret;
}


/**********************************************************************
startChat
arguments: int, char* char*
returns: void
Description: This function begins the chat with the server by sending the
first message. The client and server then alternate sendinga and receiving 
until either wants to quit. 
source: https://beej.us/guide/bgnet/html/#sendrecv
*************************************************************************/
void startChat(int sockfd, char handle[], char servHandle[]) {
	char messageIN[501];//holds the message received
	char messageOUT[501];//holds the message to send
	int bytesRec=0, bytesSent = 0;
	
	//printf("SErver handle %s\n", servHandle);
	//int *i = strchr(servHandle, '@');//find location of '@' symbol
	//*i = '\0';//replaces '@' symbol with \0
	
	
	while (1) {//loop until we break
		
		memset(messageIN, 0, sizeof(messageIN));//clear the memory
		memset(messageOUT, 0, sizeof(messageOUT));

		printf("%s>", handle);//print the clients handle
		fgets(messageOUT, sizeof(messageOUT), stdin);//get the message

		
		if (strcmp(messageOUT, "\\quit\n") == 0) {//check to see if the user wants to quit
			bytesSent = send(sockfd, messageOUT, strlen(messageOUT), 0);//tell the server we are quiting
			printf("Exiting..\n");
			break;//break out of loop
		}
		
		
		bytesSent = send(sockfd, messageOUT, strlen(messageOUT), 0);//otherwise send the message
		
		if (bytesSent == -1) {//check for sending errors
			error("Message not sent");
		}
		
		
		bytesRec = recv(sockfd, messageIN, 500, 0);//receive a messege from server
		if (bytesRec == -1) {//check for error in receiving
			error("Message not received\n");
		}
		else if (bytesRec == 0) {//check if server terminated the connection
			printf("Server Terminated connection\n");
			break;
		}
		else {//otherwise print the messesge
			printf("%s> %s\n", servHandle, messageIN);

		}

	}
	close(sockfd);


}

/***************************************************************
getNames()
This function sends the clients handle to the server then
recieves the servers handle
****************************************************************/
void getNames(int socketfd, char chandle[], char servHandle[]) {
	
	int sendClient = send(socketfd, chandle, strlen(chandle), 0);//send the client handle
	int recServe = recv(socketfd, servHandle, 10, 0);//get the server handle
	
}


int main(int argc, char *argv[]) {
	char handle[10];//client name
	char servName[10];//server handle
	int portNumber;//defined in argv[2]
	
	struct addrinfo *socketName;//struct for tcp client
	int socketfd = 0;//socket file descriptor
	if (argc != 3) {//check for valid number of arguments
		error( "Invalid number of arguments");
	}

	memset(handle, 0, sizeof(handle));
	while ((strlen(handle) > 10) || (strlen(handle)==0)) {//get the handle name
		memset(handle, 0, sizeof(handle));
		printf("Enter a 10 character username: ");
		fgets(handle, sizeof(handle), stdin);
	}
	int *i = strchr(handle, '\n');//find the \n and replace it with \0
	*i = '\0';
	//portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
	printf("Starting connection\n");
	socketName = startConnection(argv[1], argv[2]);//starts a connection uses the port number and server name
	socketfd = getSocket(socketName); //create a socket and assign it to the file descriptor
	//printf("%i", socketfd);
	int status;
	printf("connecting...");
	//source: https://beej.us/guide/bgnet/html/#connect
	if ((status = connect(socketfd, socketName->ai_addr, socketName->ai_addrlen)) != -1) {//if we can connect start the chat
		//printf("getting names\n");
		getNames(socketfd, &handle, &servName);//get the handles for the client and server
		//printf("Names: %s %s \n", handle, servName);
		printf("starting chat: \n");
		startChat(socketfd, &handle, &servName);//start the chat
	}
	else {
		error("Failed to connect with server");
	}
	freeaddrinfo(socketName);//free the memory for the struct

		return 0;
}