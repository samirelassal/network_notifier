#include "notify.h"

void notify(int, struct ethhdr *, char *, int);

int main(int argc, char const *argv[])
{    
    init(0b010000, notify);
}

void notify(int protocol, struct ethhdr *header, char *payload, int payload_size)
{
    printf("\nProtocol: %04x\nPayload:\n", protocol);
    printbuffer(payload, payload_size);
}

void printbuffer(char buffer[], int buf_len)
{
    for (int i = 0; i < buf_len; i++)
    {
        int is_div_by_four = (i+1) % 6;
        printf("%02X ", buffer[i] & 0xff);
        if (is_div_by_four == 0)
            printf("\n");
    }
    printf("\n");
}

void init(int notify_on, void (* handler)(int, struct ethhdr *, char *, int))
{
    int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_raw == -1)
    {
        printf("socket error %d\n", errno);
        return;
    }
    char buffer[IP_MAXPACKET];
    int size;
    while(1)
    {
        size = recv(sock_raw, buffer, IP_MAXPACKET, 0);
        if (size == -1)
        {
            printf("recv error %d\n",errno);
            continue;
        }
        struct ethhdr *header = (struct ethhdr *)buffer;
        int protocol = ntohs(header->h_proto);
        char *payload = (buffer + sizeof(struct ethhdr));
        int invoke = 0;
        switch(protocol)
        {
            case ETH_P_IP:
                invoke = (notify_on & 0b100000);
                break;
            case ETH_P_ARP:
                invoke = (notify_on & 0b010000);
                break;
            case 0x0842:
                invoke = (notify_on & 0b001000);
                break;
            case ETH_P_RARP:
                invoke = (notify_on & 0b000100);
                break;
            case 0x814c:
                invoke = (notify_on & 0b000010);
                break;
            case ETH_P_IPV6:
                invoke = (notify_on & 0b000001);
                break;
            default:
                invoke = (notify_on == 0b111111);                 
        }
        if (invoke)
            handler(protocol, header, payload, size - sizeof(header));
    }
    return;
}