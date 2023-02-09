/* 
 * Feito por Nico Ramos - GRR20210574 
 */

#include <stdio.h>
#include "libru.h"

typedef enum {Svac_Cmasc = -1, Svac_Smasc, Cvac_Cmasc, Cvac_Smasc} estado_t;
/* Svac_Smasc = sem vacina e sem mascara
 * Svac_Cmasc = sem vacina mas com mascara
 * Cvac_Cmasc = com vacia e com mascara
 * Cvac_Cmasc = com vacina mas sem mascara
 */

int main()
{
/* ---------------------------- Declarações ------------------------------- */

	ru_t *ru; 					

	int ticket; 		/* Índice do vetor de clientes e nº do ticket */
	estado_t estado; 	/* Estado da pessoa */

/* --------------------------- Controle do RU ------------------------------- */

	ru = inicia_ru();
	
	/* Encerra quando não tiver mais pessoas a serem atendidas
	 * ou quando acabarem as refeições */
	while (ru_aberto(ru))
	{
		/* Retira uma pessoa da fila de atendimento */
		dequeue(ru->f_atd, &ticket);
	
		/* Ve em qual estado a pessoa esta */
		estado = calcula_estado(ru, ticket);

		switch (estado)
		{
			/* Caso a pessoa esteja sem vacina */
			case Svac_Smasc:
			case Svac_Cmasc:
				nao_atendido(ru, ticket, 1);
				break;

			/* Caso a pessoa esteja vacinada e de mascara */
			case Cvac_Cmasc:
				compra_ref(ru, ticket);
				break;
			
			/* Caso a pessoa esteja vacinada mas sem mascara */
			case Cvac_Smasc:
				compra_masc(ru, ticket);
				break;
		
			default:
				break;
		}
	}

/* -------------------------------- Encerra --------------------------------- */
	
	esvazia_fila(ru);
	imprime_contab(ru);
	imprime_nao_atd(ru);
	encerra(ru);	

	return 0;
}
