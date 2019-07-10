#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <pthread.h>

void printbuffer(char [], int);

/*
void init(int notify_on, void (* handler)(int protocol, struct ethhdr *header, char **payload, int payload_size))
notify_on: describes on which protocols to notify:
    bits from left to right:
    1: IPv4
    2: ARP
    3: WoL
    4: RARP
    5: SNMP
    6: IPv6
handler: The handling function expecting two arguments
    protocol: The protocol causing this notification
    header: The header of the received package
 */
void init(int, void (* f)(int, struct ethhdr *, char *, int));