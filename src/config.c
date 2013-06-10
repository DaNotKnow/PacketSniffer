/*
*   Este codigo tem como objetivo identificar as opções de utilização passadas pelo usuário.
*/

#include<string.h>
#include<stdlib.h>
#include<config.h>


// Estrutura das opções suportadas pelo programa

static struct option loption[]=
        {
                {"smac", 1, NULL, 'S'},  // Mac da Origem
                {"dmac", 1, NULL, 'D'},  // Mac do destino
                {"sip",  1, NULL, 'I'},  // Endereco ip de origem
                {"dip",  1, NULL, 'J'},  // Endereco ip de destino
                {"sport",1, NULL, 'P'},  // Porta de origem
                {"dport",1, NULL, 'R'},  // POrta de destino
        };


// Função para fazer um "parse" das opções passadas pelo usuário.

struct inputOpt *parseOptions(int argc, char **argv)
{
	int cl_opts;				//usado para pegar o argumento passado
	char *options = "S:D:I:J:P:R:H:";	// lista de argumentos que podem ser passados e utilizados pelo getopt()
	
	// Zera todos os valores da struct
	memset(&iopt, 0, sizeof(struct inputOpt) );

	for(;;)
	{
        	// Pega o parametro passado, checa se ele esta na lista definida em "options" e seta o cl_opts com o valor do parametro
		if ((cl_opts = getopt_long(argc, argv, options, loption, NULL)) == -1)
		break;

		//Checa a opcao e seta o argumento na estrutura inputOpt que sera retornada ao final
		switch(cl_opts){
			case 'S':
				iopt.sourceMac = optarg;
				break;
			case 'D':
				iopt.destMac = optarg;
				break;
			case 'I':
				iopt.sourceIp = optarg;
				break;
			case 'J':
				iopt.destIp = optarg;
				break;
			case 'P':
				iopt.sourcePort = atoi(optarg);
				break;
			case 'R':
				iopt.destPort = atoi(optarg);
				break;

			default:
				return NULL;
		}
	}
//Retorna a estrutura de com as opções (defina no config.h)
 return &iopt;
}
