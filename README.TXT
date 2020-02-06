Steps to run program:
1. open duplicate session in linux terminal
2. Run server and type handle name (see instructions to run server)
3. In a separate window compile run the client(see instructions to run)
4. The client must intiate the chat. The Server and client take turns alternating sending messages


To run Server:
Determine server name 
Create a duplicate session to run the client on. 
type python3 chatserve.py [PORTNUM] 
where PORTNUM is a number of your choice(remember this number for the client)
Follow the prompts given and type in a handle


To compile and run client:
Determine what the server name is
run the command make (see makefile for details)
type ./chatclient [SERVERNAME] [PORTNUM]
Follow the prompts and enter your handle 
Begin the chat
