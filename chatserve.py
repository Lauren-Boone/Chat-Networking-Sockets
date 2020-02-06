#!/bin/python

#Name: LAuren Boone
#Date: 1/27/20
#Description: This is a server application used for a chat system
#A lot of the methods are taken from https://docs.python.org/2/howto/sockets.html
#https://docs.python.org/2/library/socketserver.html#socketserver-tcpserver-example
#https://docs.python.org/3/library/socket.html#creating-sockets




import sys
from socket import *
import string






#chat function
#accepts: connection, string(handle), string(cName)
#Returns none
#Description: This function waits for the client to send and message
#to the server. The server and client alternate sending and receiving messages
def chat(con, handle, cName):
    
    while 1:
        chatMessage=""#holds message from the client
        servMessage=""#holds message from the server
        #print('Receiving')
        chatMessage = con.recv(501)[0:-1]#Recieve the message from the client
        if chatMessage =="":
            break #we wait for response
        if chatMessage.decode('utf8') == "\quit": #Check if the message is to quit
            print('Ending Connection')
            break

        #https://stackoverflow.com/questions/37612100/python3-utf-8-encoding-in-http-server
        #had to use above example to receive string not binary
        print("{}>{}".format(cName.decode('utf8'),chatMessage.decode('utf8') ))#Otherwise print the message

        #Now send a message back
        servMessage="hwer"
       # while len(servMessage) > 500 and len(servMessage)<0:
       
        servMessage=input('{}>'.format(handle))
        #print(servMessage,"message")
        if servMessage == "\quit": #if we want to quit then break out of loop
            print('Ending Connection')
            break;
        else:
            con.send(servMessage.encode('utf-8')) #otherwise send the message







#getHandle()
#accepts: none
#returns: string(name)
#description: This function asks the user to specify their chat handle
def getHandle():
    name=""

    while len(name) > 10 or len(name) == 0:#while the name is greater the 10 or 0
        #prompt for input
        name=input('Please enter a username (10 characters or less):  ')

    return name




#handShake()
#accepts: con, string(servName)
#returns: clientName
#description: This function sends the servers name to the client and receives the clients name
#issue with python 3 sending in binary. https://stackoverflow.com/questions/37612100/python3-utf-8-encoding-in-http-server
# had to use example in above link to encode/decode
def handShake(con, servName):
    cName=con.recv(1024)#receive the clients name
    
    con.send(servName.encode('utf-8'))#send the servers name as a char* not binary 
    return cName







#MAIN
if __name__ == "__main__": 
    #make sure program arguments are correct
    if len(sys.argv) < 2:
        print('Incorrect number of arguments')
        exit(1)

    portNum=sys.argv[1] #Get the port number
    #taken from https://docs.python.org/3/library/socket.html#creating-sockets
    chatSocket= socket(AF_INET, SOCK_STREAM) #set up the socket with
    chatSocket.bind(('', int(portNum))) #bind the port number
    chatSocket.listen(1) #listen for one client at a time

    handle=getHandle()#get our handle name
    #print(handle)
    while 1:#stay available till close connection
        print('Ready for connections')
        connection, address=chatSocket.accept()#taken from https://docs.python.org/3/library/socket.html#creating-sockets
        clientName=handShake(connection, handle)#get the client name
        chat(connection, handle, clientName)#iniate chat
        connection.close()
