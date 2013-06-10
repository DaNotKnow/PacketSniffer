
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen
#include<sys/socket.h>
#include <arpa/inet.h>

#include <packetdump.h>
#include  <usage.h>


int main(int argc, char **argv)
{
	int sockfd, sockaddr_size, recvd_data;
	struct sockaddr sourceAddr;
	unsigned char *packet = (unsigned char *) malloc(65535); //Tamanho maximo de um pacote tcp/ip
        struct inputOpt *opt;					// Struct usada para os parametros 

 	// PEga os parametros passados e guarda na estrutura (definida no config.h)
	 opt = parseOptions(argc, argv);
	if (opt == NULL){
		usage();
		exit(1);
	}
	//Zera minha area de memoria alocada
	memset(packet, 0, sizeof packet);

	// Inicializa o socket file descriptor
	sockfd = socket( AF_PACKET, SOCK_RAW, htons(ETH_P_ALL) );
	if (sockfd == -1){
		printf("Erro ao inicar o socket\n");
		exit(1);
		}

	// "Sniffa" os pacotes ethernet e armazena no "packet"
	sockaddr_size = sizeof( struct sockaddr );

	while( (recvd_data = recvfrom(sockfd , packet , 65536 , 0 , &sourceAddr , (socklen_t*)&sockaddr_size) ))

	// Faz o dump do cabeçalho ethernet ( camada 2 OSI)
	packetDump(packet, opt);
	exit(0);
}

