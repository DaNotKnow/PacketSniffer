/*
*   Este código tem como objetivo fornecer funções de uso geral.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utils.h>


/* Funçao responsavel por converter um string em numeros hexadecimais correspondentes
*  Caso a string possua algum caracter que nao tenha um hexadecimal, a funcao retorna um ponteiro nulo (NULL)
*/

int *strtohex( char *string, int size, bool doubleNotation)
{
        int *hex = (int *)malloc( size);    	    // Variavel que armazenara os hexadecimais convertidos
        char ch;				    // Usado para conversao de caracter para decimal
        int i=0;				    // Indexador
	int j=0;			    	    // indexador
	int *doubleHex= sbrk( (size/2) +1 );        // Usei o sbrk() pois usando o malloc estava conflitando.

        for (i=0; i< size; i++)
        {


                ch = string[i];
                switch(string[i])
                {
                        case 'A':
			case 'a':
                                hex[i] = 0xa; break;

                        case 'B':
                        case 'b':
                                hex[i] = 0xb; break;

                        case 'C':
                        case 'c':
                                hex[i] = 0xc; break;

                        case 'D':
                        case 'd':
                                hex[i] = 0xd; break;

                        case 'E':
                        case 'e':
                                hex[i] = 0xe; break;

                        case 'F':
                        case 'f':
                                hex[i] = 0xf; break;

                        case '0': hex[i] = 0x0; break;
                        case '1': hex[i] = 0x1; break;
                        case '2': hex[i] = 0x2; break;
                        case '3': hex[i] = 0x3; break;
                        case '4': hex[i] = 0x4; break;
                        case '5': hex[i] = 0x5; break;
                        case '6': hex[i] = 0x6; break;
                        case '7': hex[i] = 0x7; break;
                        case '8': hex[i] = 0x8; break;
                        case '9': hex[i] = 0x9; break;

                        default: return NULL;  	           //Caso nao exista o caracter em hexa, retorna null
                }


		if (i!= 0 && ( (i+1) % 2)==0){
			/*
			* Ok...parece confuso, mas considere: hex[i-1] = 0x0A (10 em decimal); hex[i] = 0x0B (11 em decimal). 
			* Na verdade estes valores deveriam representar 0xAB (171 decimal). Para fazer isto "shiftei" em 4 posicoes os bits do primeiro hexadeimal (hex[i-1]) e adicionei ao segundo numero, ao final teremos:
			* 0xA  == 00001010 -->(shift para a direta 4x) --> 10100000 = 0xA0(160 decimal) + 0xB == 00001011 = 10101011 = 0xAB
			*/
			doubleHex[j] = (hex[i-1]<<4) + hex[i];
			j++;
		}

        }

       if ((size%2)!=0) 
       		doubleHex[j] = (hex[i-1]<<4);



if(doubleNotation == 0)
	return doubleHex;
else
	return hex;
}







