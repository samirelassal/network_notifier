#include "notify.h"

void notify(int, struct ethhdr *);

int main(int argc, char const *argv[])
{    
    init(0b111111, notify);
}

void notify(int protocol, struct ethhdr *header)
{
    printf("%04x\n", protocol);
}

void printbuffer(char buffer[], int buf_len)
{
    printf("\n");
    for (int i = 0; i < buf_len; i++)
    {
        int is_div_by_four = (i+1) % 6;
        printf("%02X ", buffer[i] & 0xff);
        if (is_div_by_four == 0)
            printf("\n");
    }
    printf("\n");
}

void init(int notify_on, void (* handler)(int, struct ethhdr *))
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
        //printbuffer(buffer, size);
        int protocol = ntohs(header->h_proto);
        switch(protocol)
        {
            case ETH_P_IP:
                if (!(notify_on & 0b100000))
                    break;
            case ETH_P_ARP:
                if (!(notify_on & 0b010000))
                    break;
            case 0x0842:
                if (!(notify_on & 0b001000))
                    break;
            case ETH_P_RARP:
                if (!(notify_on & 0b000100))
                    break;
            case 0x814c:
                if (!(notify_on & 0b000010))
                    break;
            case ETH_P_IPV6:
                if (!(notify_on & 0b000001))
                    break;
            default:
                if (notify_on & 0b111111)
                {
                    handler(protocol, header);
                }            
        }
    }
    return;
}