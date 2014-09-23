#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define FILENAMESIZE 100

int handle_connection(int sock);

int main(int argc, char * argv[])
{
    int server_port = -1;
    int rc          =  0;
    int sock        = -1;
    int socketID;



    /* parse command line args */
    if (argc != 3)
    {
	    fprintf(stderr, "usage: http_server1 k|u port\n");
	    exit(-1);
    }

    server_port = atoi(argv[2]);

    if (server_port < 1500)
    {
	    fprintf(stderr, "INVALID PORT NUMBER: %d; can't be < 1500\n", server_port);
	    exit(-1);
    }

    /* initialize */
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

    /* initialize and make socket */
    if ((socketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    { //error processing;
        printf("Failed to establish socket.\n");
        return -1;
    }


    /* set server address*/

    struct sockaddr_in saddr;

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(server_port);

    /* bind listening socket */
    if (bind(socketID, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        printf("I can't bind correctly.\n");
        return -1;
    }

    /* start listening */
    if (listen(socketID, 32) < 0)
    {
        printf("I'm a bad listener.\n");
        return -1;
    }

    /* connection handling loop: wait to accept connection */
    while (1)
    {
	    /* handle connections */
	    rc = handle_connection(sock);
    }
}

int handle_connection(int sock)
{
    bool ok = true;

    int len;
    char buf[BUFSIZE];

    const char * ok_response_f = "HTTP/1.0 200 OK\r\n"	\
	"Content-type: text/plain\r\n"			\
	"Content-length: %d \r\n\r\n";

    const char * notok_response = "HTTP/1.0 404 FILE NOT FOUND\r\n"	\
	"Content-type: text/html\r\n\r\n"			\
	"<html><body bgColor=black text=white>\n"		\
	"<h2>404 FILE NOT FOUND</h2>\n"
	"</body></html>\n";

    int new_socket = accept(sock, NULL, NULL);



    /* first read loop -- get request and headers*/
    if ((len = read(new_socket,  buf,  sizeof(buf)-1)) > 0)
    {
        printf("READING");
    }
                                              
    buf[len] = '\0';

    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (islower(buf[i]))
            buf[i] = toupper(buf[i]);
    }

    if (write(new_socket, buf, len) > 0)
    {
        printf(" writting.\n");
    }


    /* parse request to get file name */
    /* Assumption: this is a GET request and filename contains no spaces*/

    /* try opening the file */

    /* send response */
    if (ok)
    {
	/* send headers */

	/* send file */
    }
    else
    {
	// send error response
    }

    /* close socket and free space */
    close(sock);

    if (ok) return 0;
    else return -1;
}
