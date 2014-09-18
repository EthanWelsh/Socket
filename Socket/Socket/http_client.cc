/* UNCOMMENT FOR MINET
 * #include "minet_socket.h"
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#import <assert.h>
#include <unistd.h>


#define BUFSIZE 1024


int main(int argc, char * argv[])
{
    char * server_name = NULL;
    int server_port    = -1;
    char * server_path = NULL;
    char * req         = NULL;
    bool ok            = false;

    int socketID;
    int len;
    int res;
    char buf[BUFSIZE];
    struct hostent *hp;
    struct sockaddr_in saddr;

    if (argc != 5)
    {
        fprintf(stderr, "usage: http_client k|u server port path\n");
        exit(-1);
    }

    server_name = argv[2];
    server_port = atoi(argv[3]);
    server_path = argv[4];

    req = (char *)malloc(strlen("GET  HTTP/1.0\r\n\r\n")+strlen(server_path)+1);

    if (toupper(*(argv[1])) == 'K')
    {

    }
    else if (toupper(*(argv[1])) == 'U')
    {

    }
    else
    {
        fprintf(stderr, "First argument must be k or u\n");
        exit(-1);
    }


    if ((socketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    { //error processing;
        printf("Failed to establish socket.\n");
        return -1;
    }


    if ((hp = gethostbyname(server_name)) == NULL)
    {  //error processing;
        printf("DNS could not locate the page you asked for.\n");
        return -1;
    }

    saddr.sin_family = AF_INET;
    memcpy(&saddr.sin_addr.s_addr, hp->h_addr, hp->h_length);  /* TODO CHANGED LENGTH TO H_LENGTH*/
    saddr.sin_port = htons(server_port);

    printf("Connecting to Server...\n");


    if (connect(socketID, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    { //error processing;
        printf("We couldn't establish a connection\n");
        return -1;
    }

    printf("Connection has Been Established\n");


    sprintf(req, "GET %s HTTP/1.0\r\n\r\n", server_path);

    printf("Writing the following request to host:\n %s", req);

    send(socketID, req, sizeof(req) / sizeof(req[0]), 0);

    printf("Written\n");



    read(socketID, buf, BUFSIZE);

    printf("Read: %s", buf);
    printf("I READ STUFF\n");



    if (ok) {
        return 0;
    } else {
        return -1;
    }
}
