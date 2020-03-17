#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
#define NA_SI 20

void error(char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

char name[NA_SI];
void *recv_msg(void *arg);
void *send_msg(void *arg);

int main(int argv, char *argc[])
{
    int sock;
    struct sockaddr_in ser_adr;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = inet_addr(argc[1]);
    ser_adr.sin_port = htons(atoi(argc[2]));
    if (connect(sock, (struct sockaddr*)&ser_adr, sizeof(ser_adr)) == -1) 
        error("connect error");
    pthread_t send_id, recv_id;
    sprintf(name, "[%s]:", argc[3]);
    fputs(name, stdout);
    pthread_create(&send_id, NULL, send_msg, (void*)&sock);
    pthread_create(&recv_id, NULL, recv_msg, (void*)&sock);

    pthread_join(send_id, NULL);
    pthread_join(recv_id, NULL);
    close(sock);
    return 0;

}

void *send_msg(void *arg) {
    int sock = *((int*)arg);
    char msg[SIZE], ans[SIZE+NA_SI];
    while (1) {
//        fputs("INPUT:", stdout);
        fgets(msg, SIZE, stdin);
        if (strcmp(msg, "Q\n") == 0 || strcmp(msg, "q\n") == 0) {
            close(sock);
            break;
        }
        sprintf(ans, "%s %s", name, msg);
//        fputs(ans, stdout);
        write(sock, ans, sizeof(ans));
    }
    return NULL;
}
void *recv_msg(void *arg) {
    int sock = *((int *)arg);
    char msg[SIZE+NA_SI];
    int str_len = 0;
    fprintf(stdout, "%s\n", name);
    while ((str_len = read(sock, msg, NA_SI+SIZE-1)) != 0) {
        printf("%d\n", str_len);
        if (str_len == -1)
            return (void *)-1;
        msg[str_len] = 0;
//        fprintf(stdout, "%s\n", msg);
        fputs(msg, stdout);
    }
    return NULL;
}
