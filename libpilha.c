/* 
    Feito por Nico Ramos - GRR20210574
*/


#include <stdio.h>
#include <stdlib.h>

#include "libpilha.h"

pilha_t *pilha_cria(int nelem)
{
    /* Cria uma pilha e testa se deu certo */
    pilha_t *pilha;

    if (!(pilha = malloc(sizeof(pilha_t))))
        return NULL;
    
    /* Aloca e testa o vetor de elementos */
    if (!(pilha->elems = malloc(nelem*sizeof(int)))) 
    {
        free(pilha);
        pilha = NULL;
        return NULL;
    }

    /* Configura a pilha */
    pilha->topo = 0;
    pilha->nelem = nelem;

    return pilha;
}

int push(pilha_t *pilha, int elem)
{
    /* Testa se a pilha está cheia */
    if (pilha->topo == pilha->nelem)
        return -1;
 
    /* Insere elem no topo da pilha */
    pilha->elems[pilha->topo] = elem;
    (pilha->topo)++;  

    return pilha->topo;
}

int pop(pilha_t *pilha)
{  
    if (pilha_vazia(pilha))
        return 0;

    /* Remove o elem do topo e retorna ele */
    (pilha->topo)--;
    return pilha->elems[pilha->topo];
}

int pilha_topo(pilha_t *pilha)
{
    if (pilha_vazia(pilha))
        return 0;

    /* Retorna o elem do topo, mas não o remove */
    int i = pilha->topo-1;
    return pilha->elems[i];
}

int pilha_tamanho(pilha_t *pilha)
{
    return pilha->topo;
}

int pilha_vazia(pilha_t *pilha)
{
    /* Checa se o topo aponta para a primeira posição */
    if (pilha->topo == 0)
        return 1;

    return 0;
}

pilha_t *pilha_destroi(pilha_t *pilha)
{
    /* Libera e destrói o vetor de elementos */
    free(pilha->elems);
    pilha->elems = NULL;

    free(pilha);

    return NULL;
}

void pilha_imprime(pilha_t *pilha)
{
    if (!pilha_vazia(pilha))
    {
        int i;
        int tam = pilha_tamanho(pilha);

        /* Imprime até o penúltimo elemento */
        for (i = 0; i < tam-1; i++)
            printf("%d ", pilha->elems[i]);

        /* Imprime sem um espaço no final */
        printf("%d\n", pilha->elems[i]);
    }
}