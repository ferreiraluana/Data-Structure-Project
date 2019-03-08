#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
typedef struct binary_tree binary_tree;
typedef struct node node;
typedef struct queue queue;

struct binary_tree
{
    int frequency;
    unsigned char c;
    binary_tree *left;
    binary_tree *right;
};

struct node
{
    binary_tree *item;//
    node *next;
};

node* add_node(binary_tree *bt)
{
    node *new_node;
    new_node = (node*) malloc(sizeof(node));

    if(new_node == NULL) return NULL;

    new_node->item = bt;
    new_node->next = NULL;
    return new_node;
}

binary_tree* create_binary_tree(unsigned char c, int freq, binary_tree *left, binary_tree *right)
{
    binary_tree *new_binary_tree;
    new_binary_tree = (binary_tree*) malloc(sizeof(binary_tree));

    if(new_binary_tree == NULL) return NULL;

    new_binary_tree->c = c;
    new_binary_tree->frequency = freq;
    new_binary_tree->left = left;
    new_binary_tree->right = right;
    return new_binary_tree;
}

binary_tree* create_huffman_tree(unsigned bytes)

{

	//ai vem a parte da fila ou do heap,acho eu

}

void frequencia(FILE *entrada, unsigned *bytes)
{
	unsigned char c;
	while(fread(&c,1,1,entrada)) bytes[c]+=1;//le o arquivo ate o final

	rewind(entrada);// volta o arquivo ao seu comeco
}

compactando(FILE *entrada, unsigned *bytes)
{
	unsigned char c;
	frequencia(entrada,bytes);
	int i;
	for(i=0;i<256;i++)
    {
        if(bytes[i]!=0){
            //printf("%d %c\n",bytes[i],i);
        }
    }

	//binary_tree *arv =create_huffman_tree(bytes);



}

int main()
{
	FILE *entrada;
	char arq[500];//nome do arquivo e seu tipo(ex: arquivo.txt)
	scanf("%s",arq);
	entrada = fopen(arq, "rb");//fopen "chama" o arquivo, fopen("arquivo.tipo", "forma") forma- r,w,a (rb,wb,ab) binario

	if(entrada == NULL) exit(0);//verifica se o arquivo eh valido

	char caminho[20];

	scanf("%s",caminho);//ainda falta arruma, escolha para compactar ou descompactar

	unsigned bytes[256] = {0};

    compactando(entrada,bytes);

	return 0;
}
