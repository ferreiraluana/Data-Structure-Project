#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

/*

(Pre_ordem correta mas pessimo algoritmo de ordenacao)
{

	estou usando insertion sort // worst_case = O(n^2)
	a arvore esta correta, se quiser, teste no exemplo do slide Huffman
	utilizei insertion para poder ordenar o array de btrees de modo que o pre-ordem ficasse certo
	mas com certeza, heap deve ser melhor
}

//////////////////////////////////
////			      ////
////			      ////
////	  HUFFMAN TREE	      ////
////			      ////
////			      ////
//////////////////////////////////

Code by: Joao Pedro.

*/

typedef struct btree btree; //Binary Tree
typedef unsigned char byte; //Byte = 8 bits = [0|0|0|0|0|0|0|0] = que a gente usa para add o char na arvore

struct btree
{
    int frequence;
    byte c;
    btree *left;
    btree *right;
};

//
///////////
///////////////////////
///////////////////////////////////
///////////////////////////////////////////////

btree* create_btree(byte c, int freq, btree *left, btree *right); // cria um nova arvore
btree* copy_tree(btree *tree); // criar uma nova arvore igual a que ele recebe
btree* building_huff_tree(btree **trees, int t, int i); // montador da arvore
btree* create_huffman_tree(unsigned *bytes); // cria todas as arvores do array de arvores (com respectivos caracteres e frequencias)
void start_trees_array(btree **trees); // inicia o array de arvores (iguala todos a NULL)
void insertionsort(btree **trees, int c, int t); // ordena o array de arvore (ORDENADOR TEMPORARIO // FAVOR USAR HEAP DEPOIS)
void frequencia(FILE *entrada, unsigned *bytes); // elabora o array de frequencia cujo indices sao seus respectivos caracteres
void print_pre(btree *tree); // imprime a arvore de huffman em pre-ordem
void Huffman(FILE *entrada, unsigned *bytes); // funcao base para a formacao da arvore (funcao estruturadora)
int main(); // onde a magica acontece :D (PS: INT MAIN nao quero saber)

///////////////////////////////////////////////
///////////////////////////////////
///////////////////////
///////////
//

btree* create_btree(byte c, int freq, btree *left, btree *right)
{
    btree *new_btree;
    new_btree = (btree*) malloc(sizeof(btree));

    if(new_btree == NULL) return NULL;

    new_btree->c = c;
    new_btree->frequence = freq;
    new_btree->left = left;
    new_btree->right = right;
    return new_btree;
}

btree* copy_tree(btree *tree)
{
	btree *copy = (btree*) malloc(sizeof(btree));
	copy->c = tree->c;
	copy->frequence = tree->frequence;
	copy->left = tree->left;
	copy->right = tree->right;
	return copy;
}

btree* building_huff_tree(btree **trees, int t, int i)
{
	btree *sum = NULL;
	btree *right = NULL;
	btree *left = NULL;
	if(i == t-1)
	{
		return trees[i];
	}
	else
	{
		byte c = '*';
		right = copy_tree(trees[i+1]);
		left = copy_tree(trees[i]);
		sum = (btree*) malloc(sizeof(btree));
		sum->c = c;
		sum->frequence = (right->frequence + left->frequence);
		sum->left = left;
		sum->right = right;
		//free(trees[i+1]);
		//free(trees[i]);
		trees[i+1] = sum;
		i++;
		insertionsort(trees, i, t);
		building_huff_tree(trees, t, i);
	}
}

btree* create_huffman_tree(unsigned *bytes)
{
    int i,t = 1;
    btree *new_arv = NULL;
    btree *huff = NULL;
    btree *trees[257];//array de arvores que usarei para fazer a soma dps
    start_trees_array(trees);
    for(i=1;i<=256;i++)
    {
        if(bytes[i] != 0)//temos uma frequencia aqui
        {
            new_arv = (btree*) malloc(sizeof(btree));
            new_arv->frequence = bytes[i];
            new_arv->c = i;
            new_arv->left = NULL;
            new_arv->right = NULL;
            trees[t] = new_arv;
            t++;
            new_arv = NULL;
        }
    }
    insertionsort(trees, 1, t);
    puts("");
    for(i=1;i<t;i++)
    {
        printf("%c",trees[i]->c);
    }
    puts("");
    huff = building_huff_tree(trees, t, 1);
    return huff;
}

void start_trees_array(btree **trees)
{
    for(int i = 0;i<=256;i++)
    {
        trees[i] = NULL;
    }
}

void insertionsort(btree **tree, int c, int t)
{
	int i,j;
	btree *aux;
	for(i = c+1 ; i < t; i++)
	{
		aux = tree[i];
		for(j = i; (j>1) && (aux->frequence < tree[j-1]->frequence); j--)
		{
			tree[j] = tree[j - 1];
		}
		tree[j] = aux;
	}
}

void frequencia(FILE *entrada, unsigned *bytes)
{
	byte c;
	while(fread(&c,1,1,entrada))
	{
		bytes[c]+=1; // le o arquivo ate o final
	}

	rewind(entrada); // volta o arquivo ao seu comeco
}

void print_pre(btree *tree)
{
	if(tree != NULL)
	{
	    printf("%c",tree->c);
		print_pre(tree->left);
		print_pre(tree->right);

	}
}

void Huffman(FILE *entrada, unsigned *bytes)
{
	byte c;

	frequencia(entrada,bytes);//funcao made by ruanzinho para achar a frequencia

	int i;
	puts("");
	printf(" Caracteres e suas respectivas frequencias:\n\n");
	for(i = 0 ;i < 256; i++ )// printa os caracteres e suas frequencias do arquivo
    {
        if(bytes[i] != 0)
        {
            printf(" %c %d\n", i, bytes[i]);
        }
    }

	btree *huff = create_huffman_tree(bytes);

	puts("");
	printf(" Arvore de Huffman em pre_ordem:\n\n ");

	print_pre(huff);//printa em pre ordem

	puts("");
}

int main()
{
	FILE *entrada;
	char arq[500]; // nome do arquivo e seu tipo(ex: arquivo.txt)
	printf("\n Insira o do arquivo para que seja montada sua Arvore de Huffman:  nome.tipo\n\n ");
	scanf("%s",arq);
	puts("");
	entrada = fopen(arq, "rb"); // fopen "chama" o arquivo, fopen("arquivo.tipo", "forma") forma- r,w,a (rb,wb,ab) binario
	if(entrada == NULL)
	{
		exit(1); // verifica se o arquivo eh valido
	}

	char caminho[20];
	//scanf("%s",caminho); // ainda falta arrumar, escolha para compactar ou descompactar

	unsigned bytes[257] = {0};

    Huffman(entrada,bytes); // criar a arvore de huffman pelo arquivo inserido

	return 0;
}
