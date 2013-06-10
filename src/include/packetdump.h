#ifndef PACKETDUMP_H
#define PACKETDUMP_H 

#include<netinet/if_ether.h>
#include<utils.h>
#include<config.h>

// Valores usados no campo "protocol" do cabecalho ip que define o protocolo de camada 4 que o pacote ip esta usando
#define IP_ICMP 1
#define IP_TCP 6
#define IP_UDP 17

void packetDump( unsigned char *packet, struct inputOpt *opt);
void ethernetDump( unsigned char *packet);
void ipDump( unsigned char *packet);
void tcpDump( unsigned char *packet);
void udpDump( unsigned char *packet);

#endif
