#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SIZE 120
#define clien_si 520
void send_msg(char *arg, int len);
void *hand_cli(void *arg);
void error(char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
pthread_mutex_t mutex;
int cli[clien_si];
int cli_cnt;
int main(int argc, char *argv[]) {
    int sock, cli_sock;
    struct sockaddr_in ser_adr, cli_adr;
    int cli_adr_sz;
    pthread_t p_id;
    
    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&ser_adr, 0, sizeof(ser_adr));
    ser_adr.sin_family = AF_INET;
    ser_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_adr.sin_port = htons(atoi(argv[1]));
    
    if (bind(sock, (struct sockaddr*)&ser_adr, sizeof(ser_adr)) == -1) 
        error("bind error");
    if (listen(sock, 5) == -1)
        error("listen error");
    while (1) {
        cli_adr_sz = sizeof(cli_adr); 
        cli_sock = accept(sock, (struct sockaddr*)&cli_adr, (socklen_t*)&cli_adr_sz);
        
        pthread_mutex_lock(&mutex);
        cli[cli_cnt++] = cli_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&p_id, NULL, hand_cli, (void *)&cli_sock);
        pthread_detach(p_id);
        printf("connected client IP: %s \n", inet_ntoa(cli_adr.sin_addr));
    }
    close(sock);
    return 0;
}
void *hand_cli(void *arg) {
    int cli_sock = *((int *)arg);
    int str_len = 0, i;
    char msg[SIZE];

    fprintf(stdout, "yes");
    while((str_len = read(cli_sock, msg, sizeof(msg))) != 0)
        send_msg(msg, str_len);
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < cli_cnt; ++i) {
        if (cli_sock == cli[i])
           std::swap(cli[i], cli[i+1]);
    }
   --cli_cnt;
   pthread_mutex_unlock(&mutex);
   printf("client sock stop: %d\n", cli_sock);
   close(cli_sock);
   return NULL;
}
void send_msg(char *msg, int len) {
    printf("cli_cnt : %d\n", cli_cnt);
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < cli_cnt; ++i) 
        write(cli[i], msg, len);
    pthread_mutex_unlock(&mutex);
}
