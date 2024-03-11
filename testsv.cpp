#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define  PORT_NUM 50000
#define  BUF_SIZE 2048
#define  MAX_LINK 2048

void errExit(const char* error)
{
    perror(error);
    exit(EXIT_FAILURE);
}

int main(void)
{
    struct sockaddr_in svaddr;
    char buf[BUF_SIZE];
    int sfd, cld;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1)
        errExit("socket");

    memset(&svaddr, 0, sizeof(struct sockaddr_in));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(PORT_NUM);
    svaddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sfd, (const struct sockaddr*) &svaddr, sizeof(struct sockaddr_in)))
        errExit("bind");

    if(listen(sfd, MAX_LINK) == -1)
        errExit("listen");

    printf("listening...\n");

    for(;;)
    {
        cld = accept(sfd, NULL, NULL);    
        if(cld == -1)
            errExit("accept");

        recv(cld, buf, BUF_SIZE-1, 0);

        printf("Recv: %s\n", buf);

        send(cld, buf, strlen(buf), 0);

        close(cld);
        memset(buf,0,BUF_SIZE);
    }

    return 0;
}