
#ifndef CONFIG_H
#define CONFIG_H 

#include <getopt.h>
#include <linux/types.h>
#include <asm/byteorder.h>


//Estrutura que será usada para armazenas as opções fornecidas pelo usuário
static struct inputOpt {
	char *sourceMac;	       	// Mac da Origem
	char *destMac;			// Mac do destino
	char *sourceIp; 		// Endereco ip de origem
        char *destIp;			// Endereco ip de destino
	__be16  sourcePort;		// Porta de origem
        __be16  destPort;		// POrta de destino
}iopt;


struct inputOpt *parseOptions(int argc, char **argv);

#endif
