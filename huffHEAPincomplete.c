#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define MAX_SIZE 257
/*

//////////////////////////////////
////					      ////
////					      ////
////	  HUFFMAN TREE	  	  ////
////					      ////
////					      ////
//////////////////////////////////

Code by: Ruanzinho & Dedinho & Hiago
Interface by: Dedinho.

*/

typedef struct btree btree; //Binary Tree
typedef unsigned char byte; //Byte = 8 bits = [0|0|0|0|0|0|0|0] = que a gente usa para add o char na arvore
typedef struct heap heap; // Heap

struct btree
{
    int frequence;
    byte c;
    btree *left;
    btree *right;
};

struct heap
{
  int size;
  btree *data[MAX_SIZE];
};

//
///////////
///////////////////////
///////////////////////////////////
///////////////////////////////////////////////

btree* create_btree(byte c, int freq, btree *left, btree *right); // cria um nova arvore
//btree* building_huff_tree(heap *hp,btree **trees, int t,int count); // montador da arvore
btree* create_huffman_tree(unsigned *bytes); // cria todas as arvores do array de arvores (com respectivos caracteres e frequencias)
void start_trees_array(btree **trees); // inicia o array de arvores (iguala todos a NULL)
//void swap(btree *a, btree *b); // troca a posicao de duas arvores no array de arvores
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

heap* create_heap()
{
  heap *new_heap = (heap*) malloc(sizeof(heap));
  new_heap->size = 0;
  int i;
  for(i=1;i<=256;i++)
  {
    new_heap->data[i] = NULL;
  }
}

int get_parent_index(heap *hp,int i) { return (i/2); }
int left_index_(heap *hp,int i) { return (2*i); }
int right_index_(heap *hp,int i) { return (2*i + 1); }
//int what_item(heap *hp,int i) { return hp->item[i]; }

void swap(btree *a, btree *b)
{
  btree aux = *a;
  *a = *b;
  *b = aux;
}

void max_heapify(heap *hp,int i )// i = 1
{
  int biggest;
  int left_index = left_index_(hp,i); ///2
  int right_index= right_index_(hp,i); //3

  if(left_index <= hp->size && hp->data[left_index]->frequence > hp->data[i]->frequence)
  {
    biggest =  left_index;
  }
  else { biggest = i; }

  if(right_index <= hp->size && hp->data[right_index]->frequence > hp->data[biggest]->frequence)
  {
    biggest = right_index;
  }

  if(hp->data[i]->frequence != hp->data[biggest]->frequence)
  {
    swap(hp->data[i], hp->data[biggest]);
    max_heapify(hp,biggest);
  }
}

void heapsort(heap *hp)
{
  int i;
  for(i = hp->size; i>=2; i--)
  {
    swap(hp->data[1], hp->data[i]); /// talvez mudar o 1 do primeiro
    hp->size--;
    max_heapify(hp,1);
  }
}

void build_max_heap(heap *hp)
{
  int i;
  int size = hp->size;
  for(i=size/2;i>=1;i--)
  {
    max_heapify(hp,i);
  }
}

btree *dequeue(heap *hp) //// n possui a condicao de empty heap :/
{
    btree *aux = hp->data[1];
    hp->data[1] = hp->data[hp->size];
    hp->size--;
    max_heapify(hp, 1);

    return aux;
}

void enqueue(heap *hp, btree *tree)
{
    hp->size++;
    hp->data[hp->size] = tree;
    int key_index = hp->size;
    int parent_index = get_parent_index(hp, hp->size);
    while (parent_index >= 1 && hp->data[key_index]->frequence <= hp->data[parent_index]->frequence)
    {
      swap(hp->data[key_index], hp->data[parent_index]); //// ****************************
      key_index = parent_index;
      parent_index = get_parent_index(hp, key_index);
    }
}

btree *building_huff_tree(heap *hp, int t)
{
  if(hp->size == 1)
    return hp->data[1];

  btree *left = dequeue(hp);
  build_max_heap(hp);
  heapsort(hp);
  t--;
  hp->size = t;

  btree *right = dequeue(hp);
  build_max_heap(hp);
  heapsort(hp);
  t--;
  hp->size = t;

  btree *sum = (btree*) malloc(sizeof(btree));
  sum->c = '*';
  sum->frequence = (left->frequence + right->frequence);
  sum->left = left;
  sum->right = right;
  enqueue(hp,sum);

  build_max_heap(hp);
  heapsort(hp);
  t--;
  hp->size = t;

  building_huff_tree(hp,t);
}

btree* create_huffman_tree(unsigned *bytes)
{
    int i,t = 0;
    btree *aux;
    heap *hp = create_heap();
    btree *huff= NULL;

    //....................hiago....................\\
    t=1;
    for(i=1;i<=256;i++) // salvando na heap as msms coisas q tinha salvado em btree *tree[257];
    {
      if(bytes[i] != 0)
      {
          aux = create_btree(i,bytes[i],NULL,NULL);
          enqueue(hp,aux);
          t++;
      }
    }
    printf("\nAPOS ADICIONAR NA HEAP, OS NUMEROS FICAM:\n");
    for(i=1;i<=256;i++) /// teste antes do heapsort das freq na heap
    {
      if(hp->data[i] != NULL)
      {
        printf("%c %d\n", hp->data[i]->c, hp->data[i]->frequence);
      }
    }

    printf("\nDEU CERTO AEAEAEEAE SALVOU NA HEAP AAAAAAAAAAAAAA\n");

    hp->size = t;

    build_max_heap(hp);
    heapsort(hp);
    hp->size = t;
    puts("");

    printf("\nPRINT HEAPSORT:\n");
    for(i=1;i<=t;i++)
    {
      printf("%c %d\n", hp->data[i]->c, hp->data[i]->frequence);
    }


    huff = building_huff_tree(hp, t); //// master of segmentation ;-;


    return huff;
}

void start_trees_array(btree **trees)
{
    for(int i = 0;i<=256;i++)
    {
        trees[i] = NULL;
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

	frequencia(entrada,bytes); //funcao made by ruanzinho para achar a frequencia

	int i;
	puts("");
	printf(" PRINT DO ARRAY INICIAL\nCaracteres e suas respectivas frequencias:\n\n");
	for(i = 0 ;i < 256; i++ )// printa os caracteres e suas frequencias do arquivo
  {
        if(bytes[i] != 0)
        {
            printf(" %c %d\n", i, bytes[i]);
        }
  }

	btree *huff = create_huffman_tree(bytes);

	puts("");
	printf("\nArvore de Huffman em pre_ordem:\n\n ");

	print_pre(huff);//printa em pre ordem

	puts("");
}

int main()
{
	FILE *entrada;
  int numero;
	char arq[500]; // nome do arquivo e seu tipo(ex: arquivo.txt)
	printf("\n Insira o nome.tipo do arquivo para que seja montada sua Arvore de Huffman:\n\n ");
	//scanf("%s",arq);
	puts("");
	entrada = fopen("amem.txt", "rb"); // fopen "chama" o arquivo, fopen("arquivo.tipo", "forma") forma- r,w,a (rb,wb,ab) binario
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
