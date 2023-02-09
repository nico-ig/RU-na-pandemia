/* 
	Feito por Nico Ramos - GRR20210574
*/

#include <stdio.h>
#include <stdlib.h>

#include "libfila.h"

#define MAX_G 255
#define MAX_P 10

fila_t *fila_cria ()
{
	/* Cria uma fila e testa se deu certo */
	fila_t *fila;

	if (!(fila = malloc(sizeof(fila_t))))
		return NULL;

	/* Configura o nodo cabeça */
	fila->ini = NULL;
	fila->fim = NULL;
	fila->tamanho = 0;

	return fila;
}

fila_t *fila_destroi (fila_t *f)
{

	nodo_f_t *prox_pos;

	/* Só vai executar se a fila não for vazia, vai até f->ini ser NULL */
	for ( ; f->ini; f->ini = prox_pos)
	{	
		/* Guarda a próxima posição */
		prox_pos = f->ini->prox;

		/* Libera e destroi os dados do nó anterior */
		f->ini->prox = NULL;
		f->ini->chave = 0;
		free(f->ini);

		/* Atualiza o início */
		f->ini = prox_pos;
	}
	
	/* Se não tiver nenhum elemento vem direto pra cá */
	/* Libera e destrói a cabeça */
	f->fim = NULL;
	free(f);
	
	return NULL;
}

int fila_vazia (fila_t *f)
{
	/* Vê se a cabeça aponta para NULL */
	return !(f->ini);
}

int fila_tamanho (fila_t *f)
{
	return f->tamanho;
}

int queue (fila_t *f, int elemento)
{
	/* Cria um novo nodo e testa se deu certo */
	nodo_f_t* nodo;

	if (!(nodo = malloc(sizeof(nodo_f_t))))	
		return 0;

	/* Configura o nodo */
	nodo->chave = elemento;
	nodo->prox = NULL;
	
	/* Vê se é o primeiro elemento a ser adicionado */
	if (fila_vazia(f))
		f->ini = nodo;
	else
		/* Liga o último elemento ao nodo */
		f->fim->prox = nodo;

	/* Aponta o ponteiro de fim para o nodo e incrementa o tamanho */
	f->fim = nodo;
	f->tamanho++;

	return 1;
}

int dequeue (fila_t *f, int *elemento)
{
	/* Teste de underflow */
	if (fila_vazia(f))
		return 0;

	/* Aponta para o nodo a ser retirado para poder dar free no final */
	nodo_f_t *retirar;
	retirar = f->ini;

	/* FIFO */
	*elemento = retirar->chave;

	/* Atualiza fila */
	f->ini = retirar->prox;
	f->tamanho--;

	/* Libere e destrói o nodo removido */
	retirar->chave = 0;
	free(retirar);

	return 1;
}

void fila_imprime (fila_t *f)
{
	if (!fila_vazia(f))
	{
		/* Ponteiro para a posição atual */
		nodo_f_t *atual;

		/* Imprime até o penúltimo elemento */
		for(atual = f->ini; atual->prox; atual = atual->prox)
			printf("%d ", atual->chave);

		/* Imprime sem um espaço no final */
		printf("%d\n", atual->chave);
	}
}
