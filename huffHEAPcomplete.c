#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define MAX_SIZE 257
/*

/////////////////////////////////
////                         ////
////                         ////
////      HUFFMAN TREE       ////
////                         ////
////                         ////
/////////////////////////////////

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

heap* create_heap(); // cria uma heap de btrees 
btree* create_btree(byte c, int freq, btree *left, btree *right); // cria um nova arvore
btree *building_huff_tree(heap *hp, int t); // usa a heap para montar a arvore
btree* create_huffman_tree(unsigned *freq); // cria todas as arvores na heap (com respectivos caracteres e frequencias)
btree *dequeue(heap *hp); // remove o item da heap
void enqueue(heap *hp, btree *tree); // adiciona o item na heap
void max_heapify(heap *hp, int i); // 
void heapsort(heap *hp); // ordenar a heap
void build_max_heap(heap *hp); // 
int get_parent_index(heap *hp,int i); // 
int left_index_(heap *hp,int i); // 
int right_index_(heap *hp,int i); // 
void swap(btree *a, btree *b); // troca duas arvores na heap
void frequencia(FILE *entrada, unsigned *freq); // elabora o array de frequencia cujo indices sao seus respectivos caracteres
void print_pre(btree *tree); // imprime a arvore de huffman em pre-ordem
void Huffman(FILE *entrada, unsigned *freq); // funcao base para a formacao da arvore (funcao estruturadora)

///////////////////////////////////////////////
///////////////////////////////////
///////////////////////
///////////
//

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

btree *building_huff_tree(heap *hp, int t)
{
  if(hp->size == 1)
    return hp->data[1];

  btree *left = dequeue(hp); // retira o primeiro elemento da heap
  //                que no caso eh o menor
  build_max_heap(hp);
  heapsort(hp); // ordena para colocar o segundo menor
  t--;
  hp->size = t;

  btree *right = dequeue(hp); // retira o segundo menor

  build_max_heap(hp);
  heapsort(hp); // ordena novamente
  t--;
  hp->size = t;

  int freq = (left->frequence + right->frequence);
  btree *sum = create_btree('*', freq, left, right);

  enqueue(hp,sum); // adiciona a soma dos dois primeiros devolta na heap

  build_max_heap(hp);
  heapsort(hp);
  t++;
  hp->size = t;

  building_huff_tree(hp,t);
}

btree* create_huffman_tree(unsigned *freq)
{
  int i,t = 0;
  btree *aux;
  heap *hp = create_heap();
  btree *huff= NULL;

    for(i=1;i<=256;i++) // adicionando as frequencias na heap
    {
      if(freq[i] != 0)
      {
        aux = create_btree(i,freq[i],NULL,NULL);
        enqueue(hp,aux);
        t++;
      }
    }

    printf("\nAPOS ADICIONAR NA HEAP, AS FREQUENCIAS FICAM:\n");

    for(i=1;i<=256;i++) // print da heap depois de adicionados as frequencias
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
    for(i=1;i<=t;i++) // print da heap depois de ordenadas as frequencias
    {
      printf("%c %d\n", hp->data[i]->c, hp->data[i]->frequence);
    }

    huff = building_huff_tree(hp, t); // monta a arvore na heap

    return huff;
}

btree *dequeue(heap *hp) // remove o item da heap
{
  btree *aux = hp->data[1];
  hp->data[1] = hp->data[hp->size];
  hp->size--;

  max_heapify(hp, 1);

  return aux;
}

void enqueue(heap *hp, btree *tree) // adiciona o item na heap
{
  hp->size++;
  hp->data[hp->size] = tree;
  int key_index = hp->size;
  int parent_index = get_parent_index(hp, hp->size);
  while (parent_index >= 1 && hp->data[key_index]->frequence < hp->data[parent_index]->frequence)
  {
      swap(hp->data[key_index], hp->data[parent_index]);
      key_index = parent_index;
      parent_index = get_parent_index(hp, key_index);
  }
}

void max_heapify(heap *hp, int i)
{
    int biggest;
    int left_index = left_index_(hp,i);
    int right_index= right_index_(hp,i);

    if(left_index <= hp->size && hp->data[left_index]->frequence > hp->data[i]->frequence)
    {
      biggest =  left_index;
    }
    else { biggest = i; }
  
    if(right_index <= hp->size && hp->data[right_index]->frequence > hp->data[biggest]->frequence) biggest = right_index;
  
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
      swap(hp->data[1], hp->data[i]);

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

int get_parent_index(heap *hp,int i) { return (i/2); }
int left_index_(heap *hp,int i) { return (2*i); }
int right_index_(heap *hp,int i) { return (2*i + 1); }

void swap(btree *a, btree *b)
{
  btree aux = *a;
  *a = *b;
  *b = aux;
}

void frequencia(FILE *entrada, unsigned *freq)
{
  byte c;
  while(fread(&c,1,1,entrada)) // le o arquivo ate o final
  {
    freq[c]+=1; // c representa o caracter em ascii e usamos ele como indice da posicao ao qual
    //         a frequencia sera somada
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

void Huffman(FILE *entrada, unsigned *freq)
{
  byte c;

  frequencia(entrada,freq); //funcao para achar a frequencia

  int i;
  puts("");
  printf(" PRINT DO ARRAY INICIAL\nCaracteres e suas respectivas frequencias:\n\n");
  for(i = 0 ;i < 256; i++ )// printa os caracteres e suas frequencias do arquivo
  {
    if(freq[i] != 0)
    {
      printf(" %c %d\n", i, freq[i]);
    }
  }

  btree *huff = create_huffman_tree(freq); // manda o array de frequencias para que seja construida a arvore

  puts("");

  printf("\n Arvore de Huffman em pre-ordem:\n\n ");

  print_pre(huff);//printa em pre ordem

  puts("");
}

int main()
{
  FILE *entrada;
  int numero;
  char arq[500]; // variavel para guardar o nome ou diretorio do arquivo (ex: arquivo.txt)
  printf("\n Insira o nome.tipo do arquivo para que seja montada sua Arvore de Huffman:\n\n ");

  scanf("%s",arq); //Ex: C:\Users\USER_NAME\Documents\CAPCOM\aloha.txt

  puts("");
  entrada = fopen(arq, "rb"); // fopen "chama" o arquivo, fopen("arquivo.tipo", "forma") forma- r,w,a (rb, wb, ab) binario
  //                                                  (r+, w+, a+) criacao                          
  if(entrada == NULL) // verifica se o arquivo eh valido
  {
    printf("\nO arquivo digitado nao existe ou nao foi encontrado.\n");
    exit(1); // retornar 1 indica que o arquivo e invalido
  }

  //char caminho[20]; // variavel que salva a escolha
  //scanf("%s",caminho); // ainda falta arrumar, escolha para compactar ou descompactar

  unsigned freq[257] = {0}; // array que guarda as frequencias dos caracteres

    Huffman(entrada,freq); // criar a arvore de huffman pelo arquivo inserido

    return 0;
}
