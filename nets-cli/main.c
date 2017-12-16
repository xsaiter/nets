#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#include <time.h>

#include <string.h>
#include <unistd.h>

#define MAXLINE 1000

void die(const char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

void echo_client(const char *srv_ip)
{
    struct sockaddr_in srv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(1023)
    };

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (inet_pton(AF_INET, srv_ip, &srv_addr.sin_addr) <= 0) {
        die("inet_pton");
    }

    if (connect(sockfd, (struct sockaddr *) &srv_addr, sizeof (srv_addr)) < 0) {
        die("connect failed");
    }

    char send_text[MAXLINE];
    char received_text[MAXLINE];
    ssize_t nr;

    while (fgets(send_text, MAXLINE, stdin) != NULL) {

        nr = write(sockfd, send_text, strlen(send_text));
        if (nr == -1) {
            die("write");
        }

        nr = read(sockfd, received_text, MAXLINE);
        if (nr == -1) {
            die("read");
        }

        fputs(received_text, stdout);
    }
}

int main(int argc, char** argv)
{
    char *srv_ip = "127.0.0.1";

    if (argc == 2) {
        srv_ip = argv[1];
    }

    echo_client(srv_ip);

    return (EXIT_SUCCESS);
}

