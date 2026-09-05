/*
 * Minimal traceroute for musl/FreeLinX
 * Uses raw UDP sockets to send probes with increasing TTL.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_HOPS  30
#define PROBES    3
#define TIMEOUT_S 1
#define DEF_PORT  33434

static volatile sig_atomic_t done = 0;
static volatile sig_atomic_t got_alarm = 0;

static void sig_alarm(int sig) { (void)sig; got_alarm = 1; }

static unsigned short cksum(const void *buf, int len) {
    const unsigned short *p = buf;
    unsigned int sum = 0;
    while (len > 1) { sum += *p++; len -= 2; }
    if (len == 1) sum += *(const unsigned char *)p;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-m max_ttl] [-q nprobes] host\n", argv[0]);
        return 1;
    }

    int max_ttl = MAX_HOPS;
    int nprobes = PROBES;
    const char *hostname = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "m:q:")) != -1) {
        switch (opt) {
        case 'm': max_ttl = atoi(optarg); break;
        case 'q': nprobes = atoi(optarg); break;
        default:
            fprintf(stderr, "Usage: %s [-m max_ttl] [-q nprobes] host\n", argv[0]);
            return 1;
        }
    }
    if (optind >= argc) {
        fprintf(stderr, "Usage: %s [-m max_ttl] [-q nprobes] host\n", argv[0]);
        return 1;
    }
    hostname = argv[optind];

    struct addrinfo hints = { .ai_family = AF_INET, .ai_socktype = SOCK_DGRAM };
    struct addrinfo *res;
    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        fprintf(stderr, "traceroute: unknown host %s\n", hostname);
        return 1;
    }
    struct sockaddr_in dst = *(struct sockaddr_in *)res->ai_addr;
    freeaddrinfo(res);

    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd < 0) { perror("socket"); return 1; }

    int icmp_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (icmp_fd < 0) { perror("socket icmp"); return 1; }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sig_alarm;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);

    printf("traceroute to %s (%s), %d hops max\n",
           hostname, inet_ntoa(dst.sin_addr), max_ttl);

    srand(time(NULL) ^ getpid());

    for (int ttl = 1; ttl <= max_ttl && !done; ttl++) {
        printf("%2d  ", ttl);
        fflush(stdout);

        int got_reply = 0;
        for (int probe = 0; probe < nprobes && !done; probe++) {
            int port = DEF_PORT + ttl * PROBES + probe;
            dst.sin_port = htons(port);

            setsockopt(fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

            if (sendto(fd, "X", 1, 0, (struct sockaddr *)&dst, sizeof(dst)) < 0) {
                printf(" *");
                continue;
            }

            got_alarm = 0;
            alarm(TIMEOUT_S);

            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(icmp_fd, &rfds);

            int n = select(icmp_fd + 1, &rfds, NULL, NULL, NULL);
            alarm(0);

            if (n <= 0 || got_alarm) {
                printf(" *");
                continue;
            }

            char buf[1500];
            struct sockaddr_in from;
            socklen_t fromlen = sizeof(from);
            ssize_t len = recvfrom(icmp_fd, buf, sizeof(buf), 0,
                                   (struct sockaddr *)&from, &fromlen);
            if (len < 0) { printf(" *"); continue; }

            struct ip *ip = (struct ip *)buf;
            int ihl = ip->ip_hl * 4;

            if (ip->ip_p == IPPROTO_ICMP) {
                struct icmp *icmp = (struct icmp *)(buf + ihl);
                if (icmp->icmp_type == ICMP_TIME_EXCEEDED ||
                    icmp->icmp_type == ICMP_DEST_UNREACH) {
                    if (!got_reply) printf("%s ", inet_ntoa(from.sin_addr));
                    got_reply = 1;
                    if (icmp->icmp_type == ICMP_DEST_UNREACH) {
                        done = 1;
                    }
                }
            }
        }

        if (!got_reply) printf("(no reply)");
        printf("\n");
    }

    close(fd);
    close(icmp_fd);
    return 0;
}
