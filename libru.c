/*
 * Feito por Nico Ramos - GRR20210574
 */

#include <stdio.h>
#include <time.h>

#include "libru.h"

/* -------------------------------------------------------------------------  */

#define QTD_MASC 100	/* Qtd de máscaras disponíveis */
#define QTD_PESS 1000	/* Qtd de pessoas */
#define MIN_REF 500  	/* Qtd mínima de refeições */
#define MAX_REF 1000	/* Qtd máxima de refeições */

#define V_RU 1.30		/* Valor do RU */
#define V_MASC 2.50		/* Valor da máscara */
#define DIN_I 1.30		/* Quantia inferior de dinheiro */
#define DIN_S 3.80		/* Quantia superior de dinheiro */

#define PROP_VAC 9		/* Proporção de vacinados */
#define PROP_MASC 7		/* Proporção de pessoas com máscara */
#define PROP_DIN 6		/* Proporção da distribuição do dinheiro */

/* ----------------------------- Gerais ------------------------------------- */

/* Define um número aleatório no intervalo [ini, fim] e o retorna */
int aleat(int ini, int fim)
{
	/* Soma o inicio com um número no intervalo [0, tam] */
	return (ini + rand() % (fim - ini + 1));
}

/* Inicializa a contabilidade.
 * Em caso de sucesso retorna um ponteiro, se não, retorna NULL */
contab_t *inicia_contab()
{
	contab_t *contab;

	if (!(contab = malloc(sizeof(contab_t))))
		return NULL;

	contab->total = 0;
	contab->masc = 0;
	contab->ref = 0;

	return contab;	
}

/* Cria uma pessoa.
 * Retorna um ponteiro para ela em caso de sucesso, se não, retorna NULL */
pessoa_t *gera_pessoa()
{
	/* Aloca a struct pessoa e testa se deu certo */
	pessoa_t *pessoa;
	if (!(pessoa = malloc(sizeof(pessoa_t))))
		return NULL;

	/* Distribui as vacinas na proporção PROP_VAC */
	if (aleat(1, 10) <= PROP_VAC)
		pessoa->vacina = 1;
	else
		pessoa->vacina = 0;
	
	/* Distribui as mascaras na proporção PROP_MASC */
	if (aleat(1, 10) <= PROP_MASC)
		pessoa->mascara = 1;
	else
		pessoa->mascara = 0;
		
	/* Distribui o dinheiro na proporção PROP_DIN */	
	if (aleat(1, 10) <= PROP_DIN)
		pessoa->dinheiro = DIN_I;
	else 
		pessoa->dinheiro = DIN_S;

	pessoa->motv = 0;

	return pessoa;
}

/* Preenche a fila com QTD_PESS
 * Retorna 0 em caso de sucesso e 1 caso contrário */
int popula_fila(ru_t *ru)
{
	/* Gera QTD_PESS e as insere na fila de atendimento */
	int ticket;
	
	for (ticket = 0; ticket < QTD_PESS; ticket++)
	{
		/* Gera e atribui o ticket à pessoa */
		ru->clientes[ticket] = gera_pessoa();
		
		if (!(queue(ru->f_atd, ticket)))
			return 1;
	}

	return 0;
}

int ru_aberto(ru_t *ru)
{
	if (!fila_vazia(ru->f_atd) && !pilha_vazia(ru->p_ref))
		return 1;
	
	return 0;
}	

int calcula_estado(ru_t *ru, int ticket)
{
	int estado;

	estado = 2*ru->clientes[ticket]->vacina - ru->clientes[ticket]->mascara;
	return estado;
}

int compra_ref(ru_t *ru, int ticket)
{
	pessoa_t *pessoa = ru->clientes[ticket];

	/* Vẽ se a pessoa tem dinheiro e trata o erro numérico */
	if ((pessoa->dinheiro - V_RU) >= -0.01)
	{
		pop(ru->p_ref);

		pessoa->dinheiro -= V_RU;
		ru->contab->ref += V_RU;
		
		return 1;
	}

	nao_atendido(ru, ticket, 2);
	return 0;
}

int compra_masc(ru_t *ru, int ticket)
{
	pessoa_t *pessoa;

	/* Ve se tem máscaras disponiveis */
	if (!(pilha_vazia(ru->p_masc)))
	{	
		pessoa = ru->clientes[ticket];

		/* Vê se a pessoa tem dinheiro suficiente */
		if (pessoa->dinheiro >= V_MASC)
		{
			/* Reinsere a pessoa na fila com o mesmo ticket */
			if (!(queue(ru->f_atd, ticket)))
			{
				encerra(ru);
				exit(1);
			}

			pop(ru->p_masc);
	
			pessoa->mascara = 1;
			pessoa->dinheiro -= V_MASC;
			ru->contab->masc += V_MASC;

			return 1;
		}	
		/* A pessoa nao tem dinheiro para a mascara */
		nao_atendido(ru, ticket, 3);
		return 0;
	}
	/* As mascaras acabaram */
	nao_atendido(ru, ticket, 4);
	return 0;
}

int nao_atendido(ru_t *ru, int ticket, int cod)
{
	/* Insere na lista de tickets não atendidos, encerra em caso de erro */
	if (!(lista_insere_ordenado(ru->l_tck, ticket)))
	{
		encerra(ru);
		exit(1);
	}

	/* Atribui o motivo do não atendimento */
	ru->clientes[ticket]->motv = cod;
	return 1;
}

void esvazia_fila(ru_t *ru)
{
	int ticket;

	while (!(fila_vazia(ru->f_atd)))
	{	
		dequeue(ru->f_atd, &ticket);
		nao_atendido(ru, ticket, 5);
	}
}

/* Retorna a arrecadação total */
float calcula_total(ru_t *ru)
{
   	ru->contab->total = ru->contab->masc + ru->contab->ref;
	
	return ru->contab->total;
}

/* -------------------------------- Inicializa ------------------------------ */
	
ru_t *inicia_ru()
{	
	ru_t *ru;

	srand(time(0));

	if (!(ru = malloc(sizeof(ru_t))))
		encerra(ru);

	/* Inicia tudo em NULL para facilitar liberar a memória em caso de erro */
	ru->p_masc = NULL;
	ru->p_ref = NULL;
	ru->clientes = NULL;
	ru->l_tck = NULL;
	ru->contab = NULL;

	/* Aloca as estruturas e testa se deu certo */
	if (!(ru->p_masc = pilha_cria(QTD_MASC)))
		encerra(ru);
	
	if (!(ru->p_ref = pilha_cria(aleat(MIN_REF, MAX_REF))))
		encerra(ru);

	if (!(ru->clientes = malloc((sizeof(pessoa_t) * QTD_PESS))))
		encerra(ru);

	if (!(ru->f_atd = fila_cria()))
		encerra(ru);

	if (!(ru->l_tck = lista_cria()))
		encerra(ru);
		
	if (!(ru->contab = inicia_contab()))
		encerra(ru);

	if (popula_fila(ru))
		encerra(ru);

	/* Preenche as filas até o overflow */
	int i;
	for (i = 0; push(ru->p_ref, i) != -1; i++);
	for (i = 0; push(ru->p_masc, i) != -1; i++);

	return ru;
}

/* -----------------------------  Destroi e encerra ------------------------- */

/* Desaloca a memória e destrói o vetor de clientes */
void clientes_destroi(ru_t *ru)
{
	int ticket;
	for(ticket = 0; ticket < QTD_PESS; ticket++)
	{
		ru->clientes[ticket]->vacina = 0;
		ru->clientes[ticket]->mascara = 0;
		ru->clientes[ticket]->dinheiro = 0;

		free(ru->clientes[ticket]);
	}

	free(ru->clientes);
}

void encerra(ru_t *ru)
{
	/* Verifica se inicializou o ru e libera as estruturas caso iniciadas */
	if (ru)
	{
		if (ru->p_masc)
			pilha_destroi(ru->p_masc);
	
		if (ru->p_ref)
			pilha_destroi(ru->p_ref);
	
		if (ru->clientes)
			clientes_destroi(ru);
	
		if (ru->f_atd)
			fila_destroi(ru->f_atd);
	
		if (ru->l_tck)
			lista_destroi(ru->l_tck);

		if (ru->contab)
			free(ru->contab);

		free(ru);
	}
}

/* -------------------------------  Imprime --------------------------------- */

void imprime_contab(ru_t *ru)
{
	printf("\n");
	printf("Contabilidade\n");
	printf("Arrecadação total: %.2f R$\n", calcula_total(ru));
	printf("Arrecadação com as refeições: %.2f R$\n", ru->contab->ref);
	printf("Arrecadação com as máscaras: %.2f R$\n", ru->contab->masc);
	printf("\n");
}

/* Imprime os tickets não atendidos pelo motivo recebido */
void imprime_tickets(ru_t *ru, int cod)
{
	int ticket, fim;
	
	fim = lista_tamanho(ru->l_tck);
	
	printf("Tickets: ");
	for (ticket = 0; ticket < fim; ticket++)
	{
		/* Se o motivo do ticket for o recebido imprime e remove ele */
		if (ru->clientes[ticket]->motv == cod)
		{	
			printf("%d ", ticket);
			lista_retira_elemento(ru->l_tck, &ticket);
		}
	}
	printf("\n\n");

}

void imprime_nao_atd(ru_t *ru)
{
	printf("Tickets nao atendidos\n");
	printf("Quantidade: %d\n\n", lista_tamanho(ru->l_tck));
	
	printf("Motivo: pessoa não vacinada\n");
	imprime_tickets(ru, 1);

	printf("Motivo: sem dinheiro para o RU\n");
	imprime_tickets(ru, 2);

	printf("Motivo: sem dinheiro para a máscara\n");
	imprime_tickets(ru, 3);

	printf("Motivo: acabaram as máscaras\n");
	imprime_tickets(ru, 4);

	printf("Motivo: acabaram as refeições\n");
	imprime_tickets(ru, 5);
}

/* -------------------------------------------------------------------------- */
