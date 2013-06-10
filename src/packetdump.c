/*
*  Este codigo tem como objetivo fornecer as funções necessárias para filtrar e exibir os pacotes "sniffados"
*/

#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip_icmp.h>
#include<netinet/udp.h>
#include<netinet/tcp.h>
#include<netinet/ip.h>
#include<net/ethernet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/types.h>

#include<stdbool.h>
#include<arpa/inet.h>
#include<packetdump.h>
#include<utils.h>

/*************************************************************************************************************************************/
/*************************************************************************************************************************************/

int packetFilter( unsigned char *packet, struct inputOpt *opt)
{
	bool filter = 0;   // Se fora zero indica que não havera filtragem, se 1 o pacote nao deve ser exibido ( foi filtrado)
	uint32_t sourceAddr;
        uint32_t destAddr;
	int *hex;
	int sizeHex;
	int i;

	// Checa se foram passados parametros, caso nao foram, faz o dump de todos os pacotes normalmente
	if( opt->sourceMac == NULL && opt->destMac == NULL &&
	    opt->sourceIp == NULL && opt->destIp == NULL &&
	    opt->sourcePort == 0 && opt->destPort == 0
	  )
		 return filter;

	// "Monta" o pacote para analise de filtragem
	struct ethhdr *eth = (struct ethhdr *)packet;
	struct iphdr *ip =  (struct iphdr *)(packet + sizeof(struct ethhdr) );
        int iphdr_size = ip->ihl * 4;
        struct tcphdr *tcp = (struct tcphdr *)(packet + iphdr_size + sizeof(struct ethhdr) );


/* FILTRO POR MAC
**
**  Checa se os MACs de origem e destino correspondem cos os informados
**  Nota: Como fiz este programa para tambem aprender, criei minha funcao que converte arrays de caracteres(string) em hexadecimais. Esta funcao encontra-se em utils.h
*/
	if(opt->sourceMac != NULL){
		sizeHex = (int)strlen(opt->sourceMac );
        	hex = strtohex(opt->sourceMac, sizeHex,0);
		for(i=0; i< 6; i++)
		{
			if(eth->h_source[i] != hex[i]){
				filter = 1;
				break;
			}
		}
	}
	if(opt->destMac != NULL){
                sizeHex = (int)strlen(opt->destMac );
                hex = strtohex(opt->destMac, sizeHex,0);
                for(i=0; i< 6; i++)
                {
                        if(eth->h_dest[i] != hex[i]){
                                filter = 1;
                                break;
                        }
                }
        }



/* FILTRO POR IPS
**
** Checa se os IPS de origem e destino correspondem com os informados
** Nota: Fiz desta forma pois a funcao inet_pton so deve executar caso o parametro de ip de origem ou destino seja passado.
**       somente depois  de executar esta funcao posso comparar os valores passados nos parametros 
*/
	if (opt->sourceIp != NULL){
		inet_pton(AF_INET, opt->sourceIp, &sourceAddr);
		if(ip->saddr != sourceAddr )
			filter = 1;
	}
	if( opt->destIp != NULL){
		inet_pton(AF_INET, opt->destIp, &destAddr);
		if(ip->daddr != destAddr )
			filter = 1;
	};

/* FILTRO POR PORTAS 
**
** Checa se as PORTAS de origem ou destino correspondem com as informadas
*/
	if( opt->sourcePort != 0 && ntohs(tcp->source) != opt->sourcePort )
		filter = 1;
	if( opt->destPort != 0 && ntohs(tcp->dest) != opt->destPort )
                filter = 1;

	return filter;
}



/**
**    Implementar outros protocolos de camada 2 aqui.
**/
void packetDump( unsigned char *packet, struct inputOpt *opt)
{
	bool filter;
	filter = packetFilter( packet, opt);

	// So faz o dump se as condicoes de filtragem forem estabelecidas
	if (filter == 0)
	 	ethernetDump(packet);

}


//Faz o dump de pacotes Ethernet
void ethernetDump( unsigned char *packet)
{
        struct ethhdr *eth = (struct ethhdr *)packet;
        int i = 0;
       	// "Printa" as informações do MAC de origem e destino.
        printf("->MAC-SOURCE:");
        for(i =0; i< 6; i++)
		if(i<5)
        		printf("%.2X-", eth->h_source[i]);
		else
			printf("%.2X", eth->h_source[i]);

        printf("\tMAC-DEST:");
       	for(i =0; i< 6; i++)
		if(i<5)
	               	printf("%.2X-", eth->h_dest[i]);
		else
			printf("%.2X", eth->h_source[i]);

        // Checa qual o protocolo de camada 3 (OSI) que sera usado. Novas protocolos devem ser incluidos aqui
       	switch( ntohs(eth->h_proto) ){
                	case ETH_P_IP:                                  // Caso seja um protocolo de internet (maiores detalhes veja o if_ether.h)
                       	ipDump(packet); 	                // Faz o dump do pacote ip
                      	 break;

			default:
        	               printf(" \tProtocolo de rede nao implementado...aguarde novas versoes\n"); // Micro$oft, mude aqui para "compre a versao full" xD
  	}

}


//Faz o dump de pacotes IP
void ipDump( unsigned char *packet)
{
        struct iphdr *ip =  (struct iphdr *)(packet + sizeof(struct ethhdr) );          // O ponteiro para a estrutura do cabecalho ip apontará para packet + o tamanho do cabec$
        struct sockaddr_in source, dest;
        char sourceAddr[INET_ADDRSTRLEN];
        char destAddr[INET_ADDRSTRLEN];

        // "Zera" as structs na stack para garantir que nao possuam outros dados
        memset(&source, 0, sizeof(struct sockaddr_in) );
        memset(&dest, 0, sizeof(struct sockaddr_in) );

        // Pega os enderecos de origem e destino do pacote e colocam na struct sockaddr_in
        source.sin_addr.s_addr = ip->saddr;
        dest.sin_addr.s_addr = ip->daddr;

        // "Traduz" os endereços de origem e destino da forma passada pela rede para a forma "legivel". Sim, eu poderia fazer o cast e evitar as 2 linhas acima, porem achei mai$
        inet_ntop(AF_INET, &source.sin_addr.s_addr, sourceAddr, sizeof(sourceAddr) );
        inet_ntop(AF_INET, &dest.sin_addr.s_addr, destAddr, sizeof(sourceAddr) );

        // Printa os enderecos de origem e destino
        printf("\tIP-ORIGEM:%s\tIP-DESTINO:%s", sourceAddr, destAddr);

        // Checa o protocolo de camada 4 (OSI) que o pacote esta usando. Novos protocolos devem ser implementados aqui
        switch(ip->protocol){
                case IP_TCP:
                        tcpDump(packet);
                        break;

		case IP_UDP:
			udpDump(packet);
			break;

                default:
                        printf("  \tProtocolo de transporte nao implementado...aguarde novas versoes\n");    //Prato cheio tio bill
        }

}


//Faz o dump dos pacotes TCP
void tcpDump( unsigned char *packet)
{
        /*
        *   Usei esta tecnica para achar o tamanho exato do cabecalho ip. Eu poderia (em teoria)  calcular o tamanho pelo sizeof(struct iphdr)
        *   porem, nas vezes que eu testei percebi que varios erros de referencia do ponteiro tcp. Ao pegar o tamanho do cabecalho declarado no proprio  pacote
        *   consegui sempre estar alinhado com o ponteiro tcp.
        */
        struct iphdr *ip =  (struct iphdr *)(packet + sizeof(struct ethhdr) );
        int iphdr_size = ip->ihl * 4;
        struct tcphdr *tcp = (struct tcphdr *)(packet + iphdr_size + sizeof(struct ethhdr) );

       	// Printa as portas de origem e destino
        printf("\tTCP:PORTA-ORIGEM:%u\tTCP:PORTA-DESTINO:%u\n", ntohs(tcp->source), ntohs(tcp->dest) );

}

//Faz o dump dos pacotes UDP
void udpDump( unsigned char *packet)
{
	struct iphdr *ip = (struct iphdr *)(packet + sizeof(struct ethhdr) );
	int iphdr_size = ip->ihl * 4;
	struct udphdr *udp = (struct udphdr *)(packet + iphdr_size + sizeof(struct ethhdr) );

	printf("\t UDP:PORTA-ORIGEM:%u\tUDP:PORTA-DESTINO:%u\n",ntohs(udp->source), ntohs(udp->dest) );

}

