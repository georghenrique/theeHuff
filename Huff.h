#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//Definição do tipo de dados 'byte'
//'unsigned char': É o tipo que consegue gravar no intervalo que vai de 0 a 255 bytes

typedef unsigned char byte;

//Definição da árvore
typedef struct nodeArvore
{
    int frequencia;
    byte c;
    struct nodeArvore *esquerda;
    struct nodeArvore *direita;
}nodeArvore;

//Definição da fila de prioridade (implementada como lista simplesmente encadeada)
typedef struct nodeLista
{
    nodeArvore *n;
    struct nodeLista *proximo;
}nodeLista;

typedef struct lista
{
    nodeLista *head;
    int elementos;
}lista;

