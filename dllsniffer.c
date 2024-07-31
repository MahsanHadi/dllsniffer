#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_PACKET_SIZE 65536

void print_packet(const unsigned char *packet, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", packet[i]);
    }
    printf("\n");
}

void determine_packet_type(struct sockaddr_ll *sll) {
    switch (sll->sll_pkttype) {
        case PACKET_OUTGOING:
            printf("Outgoing:\n");
            break;
        case PACKET_HOST:
            printf("Incoming:\n");
            break;
        case PACKET_BROADCAST:
            printf("Broadcast:\n");
            break;
        case PACKET_MULTICAST:
            printf("Multicast:\n");
            break;
        default:
            printf("Unknown:\n");
            break;
    }
}

int main() {
    int sockfd;
    unsigned char buffer[MAX_PACKET_SIZE];
    struct sockaddr_ll phyaddr;
    socklen_t phyaddr_len = sizeof(struct sockaddr_ll);

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    while (1) {
        int packet_len = recvfrom(sockfd, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&phyaddr, &phyaddr_len);
        if (packet_len < 0) {
            perror("Failed to receive packets");
            close(sockfd);
            return EXIT_FAILURE;
        }

        determine_packet_type(&phyaddr);

        print_packet(buffer, packet_len);
    }

    close(sockfd);
    return EXIT_SUCCESS;
}
