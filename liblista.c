/* 
	Feito por Nico Ramos - GRR20210574
*/

#include <stdio.h>
#include <stdlib.h>

#include "liblista.h"

lista_t *lista_cria ()
{
	/* Cria uma lista e testa se deu certo */
	lista_t *lista;

	if (!(lista = malloc(sizeof(lista_t))))
		return NULL;

	/* Configura o nodo cabeça */
	lista->ini = NULL;
	lista->tamanho = 0;

	return lista;
}

lista_t *lista_destroi (lista_t *l)
{
	nodo_l_t *prox_pos;

	/* Só vai executar se a lista não for vazia, vai até l->ini ser NULL */
	for ( ; l->ini; l->ini = prox_pos)
	{	
		/* Guarda a próxima posição */
		prox_pos = l->ini->prox;

		/* Libera e destroi os dados do nó anterior */
		l->ini->prox = NULL;
		l->ini->elemento = 0;
		free(l->ini);

		/* Atualiza o início */
		l->ini = prox_pos;
	}
	
	/* Se não tiver nenhum elemento vem direto pra cá */
	/* Libera a cabeça */
	free(l);
	
	return NULL;
}

int lista_vazia (lista_t *l)
{
	/* Vê se a cabeça aponta para NULL */
	return !(l->ini);
}

int lista_tamanho (lista_t *l)
{
	return l->tamanho;
}

int lista_insere_inicio (lista_t *l, int elemento)
{
	/* Cria um novo nodo e testa se deu certo */
	nodo_l_t *nodo;

	if (!(nodo = malloc(sizeof(nodo_l_t))))
		return 0;

	/* Configura o novo nodo */
	nodo->elemento = elemento;

	/* Vê se é o primeiro elemento a ser adicionado */
	if (lista_vazia(l))
		nodo->prox = NULL;
	else
		/* Aponta o nodo para o primeiro elemento da lista */
		nodo->prox = l->ini;

	/* Atualiza a lista */
	l->ini = nodo;
	l->tamanho++;

	return 1;
}

int lista_insere_fim (lista_t *l, int elemento)
{
	/* Cria um novo nodo e os ponteiros auxiliares*/
	nodo_l_t *nodo, *fim, *ultimo;

	/* Aloca e checa se deu certo */
	if (!(nodo = malloc(sizeof(nodo_l_t))))
		return 0;

	/* Configura o novo nodo */
	nodo->prox = NULL;
	nodo->elemento = elemento;

	l->tamanho++;

	/* Vê se é o primeiro elemento a ser adicionado */
	if (lista_vazia(l))
		l->ini = nodo;
	else
	{
		/* Procura o último nó, para quando fim for NULL */
		for (fim = l->ini; fim; fim = fim->prox)
			ultimo = fim;

		/* Adiciona o nodo ao final */
		ultimo->prox = nodo;
	}

	return 1;
}

int lista_insere_ordenado (lista_t *l, int elemento)
{
	/* Cria um novo nodo e testa se deu certo */
	nodo_l_t *nodo;
	
	if (!(nodo = malloc(sizeof(nodo_l_t))))
		return 0;

	/* Configura o nodo */
	nodo->elemento = elemento;
	nodo->prox = NULL;

	l->tamanho++;

	/* Vê se é o primeiro elemento a ser adicionado */
	if (lista_vazia(l))
		l->ini = nodo;

	/* Vê se é o menor elemento e o insere no início */
	else if (l->ini->elemento >= elemento)
	{
		/* Reconfigura a lista */
		nodo->prox = l->ini;
		l->ini = nodo;
	}
	
	/* Caso o elemento seja inserido no meio da lista */
	else
	{
		/* Cria dois ponteiros auxiliares */
		nodo_l_t *atual, *anterior;

		anterior = l->ini; /* Aponta para o primeiro elem */
		atual = l->ini->prox; /* Aponta para o segundo elem */

		/* Percorre a lista até inserir ou atual ser NULL */
		for ( ; atual; atual = atual->prox)
		{
			/* Se o atual for maior, insere o nodo na posição anterior */
			if (atual->elemento > elemento)
			{
				/* Reconfigura a lista */
				nodo->prox = atual;
				anterior->prox = nodo;

				return 1;
			}

			anterior = atual;
		}

		/* Se chegar até aqui é porque o elemento é o maior da lista */
		anterior->prox = nodo;
	}

	/* Valor de retorno para lista vazia, ou para o menor ou maior elemento */
	return 1;
}

int lista_retira_inicio (lista_t *l, int *elemento)
{
	/* Teste de underflow */
	 if (lista_vazia(l))
	 	return 0;
	 else
	 {
	 	/* Cria um nodo para armazenar a próxima posição */
	 	nodo_l_t *prox_pos = l->ini->prox; 

	 	/* Elemento recebe o primeiro elemento da lista */
	 	*elemento = l->ini->elemento;

	 	/* Destrói e desaloca o primeiro nodo */
	 	l->ini->elemento = 0;
	 	l->ini->prox = NULL;
	 	free(l->ini);

	 	/* Atualiza a lista */
	 	l->ini = prox_pos;
	 	l->tamanho--;

	 	return 1;
	 }
}

int lista_retira_fim (lista_t *l, int *elemento)
{
	/* Teste de underflow */
	if (lista_vazia(l))
		return 0;

	/* Cria um nodo para marcar a última posição */
	nodo_l_t *ultimo = l-> ini;

	if (lista_tamanho(l) == 1)
		l->ini = NULL;

	/* Caso tenha mais de um elementos */
	else
	{
		nodo_l_t *anterior;

		/* Encrontra o último elemento da lista */
		for( ; ultimo->prox; ultimo = ultimo->prox)
			anterior = ultimo;

		/* Aponta o penúltimo elemento para NULL */
		anterior->prox = NULL;
	}

	/* Elemento recebe o último elemento da lista */
	*elemento = ultimo->elemento;

	/* Destrói e desaloca o último nodo */
	ultimo->elemento = 0;
	free(ultimo);

	l->tamanho--;

	return 1;
}

int lista_retira_elemento (lista_t *l, int *elemento)
{
	if (!lista_vazia(l))
	{
		/* Vê se é o primeiro elemento */
		if (l->ini->elemento == *elemento)
		{
			/* Cria um ponteiro para o nodo a ser retirado */
			nodo_l_t *aux = l->ini; 

			/* Reconfigura a lista */
			l->ini = l->ini->prox;
			l->tamanho--;

			/* Libera e destrói o nodo */
			aux->elemento = 0;
			free(aux);

			return 1;
		}

		/* Cria dois ponteiros auxiliares */
		nodo_l_t *atual, *anterior;

		anterior = l->ini; /* Aponta para o primeiro elem */
		atual = l->ini->prox; /* Aponta para o segundo elem */

		/* Percorre a lista ou até achar o elemento ou atual for NULL */
		for ( ; atual; atual = atual->prox)
		{
			if (atual->elemento == *elemento)
			{
				/* Aponta o nodo anterior para posição seguinte a do elemento */
				anterior->prox = atual->prox;

				/* Destrói e desaloca o nodo que contém o elemento */
				atual->elemento = 0;
				atual->prox = NULL;
				free(atual);

				l->tamanho--;

				return 1;
			}

			anterior = atual;
		}
	}

	/* Se chegou até aqui a lista ou é vazia ou o elemento não pertence */
	return 0;
}

int lista_pertence (lista_t *l, int elemento)
{
	if (!lista_vazia(l))
	{
		nodo_l_t *atual;

		/* Percorre a lista ou até achar o elemento ou atual for NULL */
		for (atual = l->ini; atual; atual = atual->prox)
			if (atual->elemento == elemento)
				return 1;
	}


	/* Se chegou até aqui a lista ou é vazia ou o elemento não pertence */
	return 0;
}

void lista_imprime (lista_t *l)
{
	if (!lista_vazia(l))
	{
		/* Ponteiro para a posição atual */
		nodo_l_t* aux;

		/* Imprime até o penúltimo elemento */
		for(aux = l->ini; aux->prox; aux = aux->prox)
			printf("%d ", aux->elemento);

		/* Imprime sem um espaço no final */
		printf("%d\n", aux->elemento);
	}

}
