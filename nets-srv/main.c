#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <time.h>

#include <string.h>
#include <unistd.h>

#define MAXLINE 1000
#define LISTENQ 10

void die(const char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

void echo_srv()
{    
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(1023)
    };

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        die("opening socket");
    }

    if (bind(listen_fd, (struct sockaddr*) &addr, sizeof (addr)) < 0) {
        die("binding");
    }

    if(listen(listen_fd, LISTENQ) < 0){
        die("listen");
    }
    
    while (1) {
        int conn_fd = accept(listen_fd, NULL, NULL);
        if (conn_fd < 0) {
            die("accept");
        }

        pid_t pid = fork();
        if (pid == 0) { /*child*/
            close(listen_fd);

            char buf[MAXLINE];

            ssize_t nr;

            while ((nr = read(conn_fd, buf, MAXLINE)) > 0) {
                nr = write(conn_fd, buf, strlen(buf));
                if(nr == -1){
                    die("write");
                }
                memset(buf, 0, MAXLINE);
            }

            exit(0);
        }

        close(conn_fd);
    }
}

int main(int argc, char** argv)
{
    echo_srv();
    return (EXIT_SUCCESS);
}

