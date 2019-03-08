#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
typedef struct btree btree; //Binary Tree
typedef struct node node;
typedef struct queue queue; // pra que esse queue?
typedef unsigned char byte;

struct btree
{
    int frequence;
    byte c;
    btree *left;
    btree *right;
};

struct node
{
    btree *item;
    node *next;
};

node* add_node(btree *bt)
{
    node *new_node;
    new_node = (node*) malloc(sizeof(node));

    if(new_node == NULL) return NULL;

    new_node->item = bt;
    new_node->next = NULL;
    return new_node;
}

btree* create_btree(byte c, int freq, btree *left, btree *right)
{
    btree *new_btree;
    new_btree = (btree*) malloc(sizeof(btree));

    if(new_btree == NULL) return NULL;

    new_btree->c = c;
    new_btree->frequency = freq;
    new_btree->left = left;
    new_btree->right = right;
    return new_btree;
}

btree* create_huffman_tree(unsigned bytes)

{

	//ai vem a parte da fila ou do heap,acho eu

}

void frequencia(FILE *entrada, unsigned *bytes)
{
	byte c;
	while(fread(&c,1,1,entrada)) bytes[c]+=1;//le o arquivo ate o final

	rewind(entrada);// volta o arquivo ao seu comeco
}

void compactando(FILE *entrada, unsigned *bytes)
{
	byte c;
	frequencia(entrada,bytes);
	int i;
	for(i=0;i<256;i++)
    {
        if(bytes[i]!=0){
            //printf("%d %c\n",bytes[i],i);
        }
    }

	//btree *arv =create_huffman_tree(bytes);



}

int main()
{
	FILE *entrada;
	char arq[500];//nome do arquivo e seu tipo(ex: arquivo.txt)
	scanf("%s",arq);
	entrada = fopen(arq, "rb");//fopen "chama" o arquivo, fopen("arquivo.tipo", "forma") forma- r,w,a (rb,wb,ab) binario
	if(entrada == NULL) exit(0);//verifica se o arquivo eh valido

	char caminho[20];

	scanf("%s",caminho);//ainda falta arrumar, escolha para compactar ou descompactar

  if(!strcmp(caminho,"c"))
  {

  }

	unsigned bytes[256] = {0};

  compactando(entrada,bytes);

	return 0;
}
