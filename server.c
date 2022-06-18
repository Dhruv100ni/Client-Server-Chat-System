#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//netinet is a library that contains definitions for internet protocol 

//perror is a function that prints the error message to stderr


void error(const char *msg) {
  perror(msg);              //Inverse Function which Interprets the error message outputs the error description by STD e RR 
  exit(1);                  //To terminate programm
}      


int main(int argc, char *argv[]){
    //argc is the number of arguments passed to the program
    //argv is an array of pointers to the arguments passed to the program

    //bind is a function that binds a socket to a local address and port
    //connect is a function that connects a socket to a remote address and port
    //listen is a function that listens for connections on a socket

    //listener is a socket that listens for incoming connections on a specified port number and address family 
    //talker is a socket that initiates a connection to a remote host on a remote port and returns a socket descriptor for the connection 



    if (argc < 2){
        fprintf(stderr, "Port is not Provided. Program terminated\n";
        exit(1);
    }

    // stockfd is the file descriptor for the socket
    // newsockfd is the file descriptor for the new connection

    int sockfd, newsockfd, portno, n;       
    char buffer[255];

    //sockaddr is a structure that contains an address
    //sockaddr_in is a structure that contains an address for the internet
    //socklen_t is a type for the length of a sockaddr structure 32-bit datatype
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    //AF_INET is the address domain of the socket
    //SOCK_STREAM is the type of socket
    //0 is the protocol number
    //socket returns the file descriptor for the socket
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    //atoi converts a string to an integer
    //argv[1] is the port number
    //argv[0] is the name of the program
    //argc is the number of arguments passed to the program (including the name of the program) and is the number of arguments

    portno = atoi(argv[1]);

    //setsockopt is used to set the options for the socket
    //htons converts a host short integer to a network short integer
    //sin_family is the address family

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //bind is used to bind the socket to the address and port number specified in the sockaddr structure and the
    // length of the structure is passed as an argument to the function in the second argument to bind function

    //serv_addr is the address to bind to the socket to the address of the server and port number provided by the user 
    //sockaddr_in is the structure that contains the address

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    //listen is used to listen for connections
    //clilen is the length of the client address

    listen(   sockfd,5);
    clilen = sizeof(cli_addr);

    //newsockfd is the file descriptor for the new connection 

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0)
        error("ERROR on accept");

    while(1){

        //bzero is used to clear the buffer before reading from the socket and writing to the socket 
        bzero(buffer,255);
        
        n = read(newsockfd,buffer,255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Client : %s\n",buffer);

        //fgets is used to read from the keyboard and store the data in the buffer
        bzero(buffer,255);
        fgets(buffer,255,stdin);

        //write is used to write to the socket and send the data to the client
        n = write(newsockfd,buffer,strlen(buffer));

        if (n < 0)
            error("ERROR writing to socket");

        //strcmp is used to compare two strings
        int l = strncmp("Bye", buffer, 3);
        if(l == 0) {
            break;
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}