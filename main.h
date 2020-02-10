#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <pthread.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

typedef struct {
    int thread_id;
    unsigned long attacked_addr;
} IPinfo;

int cldap_amplification(IPinfo, int);
int http_flood(IPinfo, int);
int tcp_conn_flood(IPinfo, int);