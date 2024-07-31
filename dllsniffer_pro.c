#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/if_ether.h>
#include <netinet/ip.h>

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

void determine_upper_layer(const unsigned char *buffer) {
    struct ethhdr *eth = (struct ethhdr *)buffer;
    char *protocol;

    if (ntohs(eth->h_proto) == ETH_P_IP) {
        protocol = "IP";

        struct iphdr *ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
        printf("Upper Protocol: %s (header len: %d, total len: %d, proto: %d), ", protocol, ip->ihl * 4, ntohs(ip->tot_len), ip->protocol);
    } else if (ntohs(eth->h_proto) == ETH_P_ARP) {
        protocol = "ARP";
        printf("Upper Protocol: %s, ", protocol);
    } else {
        protocol = "Other";
        printf("Upper Protocol: %s, ", protocol);
    }
}

int main() {
    int packet_socket;
    unsigned char buffer[MAX_PACKET_SIZE];
    struct sockaddr_ll phyaddr;
    socklen_t phyaddr_len = sizeof(struct sockaddr_ll);

    packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (packet_socket < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    while (1) {
        int packet_len = recvfrom(packet_socket, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&phyaddr, &phyaddr_len);
        if (packet_len < 0) {
            perror("Failed to receive packets");
            close(packet_socket);
            return EXIT_FAILURE;
        }

        determine_upper_layer(buffer);

        determine_packet_type(&phyaddr);

        print_packet(buffer, packet_len);
    }

    close(packet_socket);
    return EXIT_SUCCESS;
}
