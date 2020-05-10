// Sky Hoffert
// Server for TCP-hole-punching.

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

struct args_t {
    char* ip;
    int port;
    int myport;
};

int ParseArgs(struct args_t* a, int argc, char* argv[]);

int main(int argc, char* argv[])
{
    int eno = 0;
    struct args_t* args = malloc(sizeof(struct args_t));
    
    eno = ParseArgs(args, argc, argv);
    if (eno != 0) {
        fprintf(stderr, "E. Got error code %d on arg parse.\n", eno);
        return eno;
    }

    printf("Sending to: %s:%d on port %d\n", args->ip, args->port, args->myport);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "E. Error creating socket.\n");
        return 1;
    }

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        perror("setsockopt.");
        return 1;
    }

    struct sockaddr_in my_addr, peer_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = inet_addr(args->ip);
    my_addr.sin_port = htons(args->myport);

    if (bind(sock, (struct sockaddr*) &my_addr, sizeof(my_addr)) != 0) {
        perror("bind.");
        return 1;
    }

    if (listen(sock, 1) != 0) {
        perror("listen.");
        return 1;
    }

    int addrsize = sizeof(peer_addr);
    int rxsock = accept(sock, (struct sockaddr*) &peer_addr, (socklen_t*)&addrsize);

    if (rxsock < 0) {
        perror("accept.");
        return 1;
    }

    printf("Got a connection!\n");

    // TODO

    printf("Finished.\n");

    close(sock);

    return 0;
}

// Returns 0 on successful argparse, error code otherwise.
int ParseArgs(struct args_t* a, int argc, char* argv[]) {
    if (argc != 4) {
        return 1;
    }

    char* buf[sizeof(struct in6_addr)];
    int eno = inet_pton(AF_INET, argv[1], buf);

    if (eno != 1) {
        return 2;
    }

    a->ip = argv[1];

    a->port = atoi(argv[2]);

    if (a->port < 1024) {
        return 3;
    }

    a->myport = atoi(argv[3]);

    if (a->myport < 1024) {
        return 4;
    }

    return 0;
}