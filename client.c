#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

void error(const char *msg) {
  perror(msg);
  exit(0);
}

int main (int argc, char *argv[])
{
    //hostent is a structure that contains information about a host and its addresses in the host table 


    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[255];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    //sock_stream is the type of socket 
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //h_addr is a pointer to a char array that contains the IP address in ASCII form 
    //s_addr is the address in network byte order
    //bzero is a function that sets all the bytes in a buffer to 0
    //bcopy is a function that copies n bytes from src to destination 

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    
    while(1) {

        //stdin is a file descriptor for the standard input

        bzero(buffer,255);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");
        bzero(buffer,255);
        n = read(sockfd,buffer,255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Server : %s\n",buffer);

        int l = strncmp("Bye", buffer, 3);
        if(l == 0) {
            break;
        }
    }

    close(sockfd);
    return 0;

    
}
