#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <CUnit/Basic.h>

//gcc  -o huffman huffman.c -lm -lcunit

#define MAX_SIZE 257

/*
/////////////////////////////////
////                         ////
////                         ////
////          HUFFMAN        ////
////                         ////
////                         ////
/////////////////////////////////
*/

// FINISHED

typedef struct btree btree; //Binary Tree
typedef unsigned char byte; //Byte = 8 bits = [0|0|0|0|0|0|0|0] = que a gente usa para add o char na arvore
typedef short two_bytes; // 2 Bytes = 16 bits = [0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0] = usar no cabecalho para guardar o espaco de 2 bytes
typedef struct heap heap; // Heap
typedef struct hash_table hash; // dictionary
typedef struct element element; // dictionary Item

struct btree
{
  long long int frequence;
  byte c;
  btree *left;
  btree *right;
};

struct heap
{
  long long int size;
  btree *data[MAX_SIZE];
};

struct element
{
  byte binary[100];
};

struct hash_table
{
  element *table[256];
};

//
///////////
///////////////////////
///////////////////////////////////
///////////////////////////////////////////////

hash* create_dictionary(); // cria o dicionario
heap* create_heap(); // cria uma heap de btrees
btree* create_btree(byte c, long long int freq, btree *left, btree *right); // cria um nova arvore
btree *building_huff_tree(heap *hp, long long int t); // usa a heap para montar a arvore
btree* create_huffman_tree(unsigned *freq); // cria todas as arvores na heap (com respectivos caracteres e frequencias)
btree* Huffman(FILE *input, unsigned *freq, unsigned *tam); // funcao base para a formacao da arvore (funcao estruturadora)
btree *dequeue(heap *hp); // remove o item da heap
void enqueue(heap *hp, btree *tree); // adiciona o item na heap
void max_heapify(heap *hp, long long int i); // funcao complementar da heapsort()
void heapsort(heap *hp); // ordenar a heap
void build_max_heap(heap *hp); // funcao complementar da heapsort()
long long int get_parent_index(heap *hp,long long int i); // funcao complementar da heapsort()
long long int left_index_(heap *hp,long long int i); // funcao complementar da heapsort()
long long int right_index_(heap *hp,long long int i); // funcao complementar da heapsort()
void swap(btree *a, btree *b); // troca duas arvores na heap
void frequencia(FILE *input, unsigned *freq); // elabora o array de frequencia cujo indices sao seus respectivos caracteres
void print_pre(btree *tree, unsigned *tam); // imprime a arvore de huffman em pre-ordem e soma o numero de nodes
byte* add_l(byte *binary, long long int *i); // funcao complementar da Hash()
byte* add_r(byte *binary, long long int *i); // funcao complementar da Hash()
void put(hash *hs, byte indice, byte *binary); // adiciona o binario do caracter no dicionario
void Hash(hash *hs, btree *arv, byte *binary, long long int *i); // funcao base da criacao do dicionario
void print_dictionary(hash *hs); // printa o dicionario por ordem da tabela ASCII
byte set_bit(byte c, long long int i); // seta o bit em uma determinada posicao do byte
void make_header(byte *bits, byte one[0], byte two[0]); // monta os 2 bytes do cabecalho
byte *Dec_to_binary(long long int n, byte *binary, long long int size); // transforma um inteiro em binario e coloca esse binario em uma string
byte *make_pre_order(btree *huff, long long int *cont, byte *pre_order); // monta a pre-ordem da arvore considerando se a folha eh '*' ou '\'
FILE *construct_file(FILE *input, hash *dictionary, byte *pre_order, byte *bits, int size, byte c); // constroi o arquivo comprimido (COM CABECALHO)
int is_bit_i_set(byte c, long long int i); // verifica se o bit da posicao i estah setado
btree *tree_from_pre_order(btree *huff, byte *pre_order, int *cont); // monta a arvore pela pre-ordem dada no arquivo
FILE *construct_file_decompress(FILE *input, btree *huff,int tam_cabecalho, int trash); // funcao para construir o arquivo descompactado usando a arvore
FILE *Decompress(FILE *input, btree *huff); // funcao base para a descompressao e leitura do cabecalho

///////////////////////////////////////////////
///////////////////////////////////
///////////////////////
///////////
//

hash* create_dictionary()
{
  hash *new_hash = (hash*) malloc(sizeof(hash));
  int i;
  for (i = 0; i < 256; i++)
  {
    new_hash->table[i] = NULL;
  }
  return new_hash;
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

btree* create_btree(byte c, long long int freq, btree *left, btree *right)
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

btree *building_huff_tree(heap *hp, long long int t)
{
  if(hp->size == 1) return hp->data[1];

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

  long long int freq = (left->frequence + right->frequence);
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
  unsigned i;
  long long int t = 0;
  btree *aux;
  heap *hp = create_heap();
  btree *huff= NULL;

    for(i = 0;i <= 256;i++) // adicionando as frequencias na heap // tava 1
    {
      if(freq[i] != 0)
      {
        aux = create_btree(i,freq[i],NULL,NULL);
        enqueue(hp,aux);
        t++;
      }
    }

    printf("\n Apos adicionar na heap, as frequencias ficam:\n\n");

    for(i = 1;i <= 256;i++) // print da heap depois de adicionados as frequencias
    {
      if(hp->data[i] != NULL)
      {
        if(hp->data[i]->frequence != 0)
        {
          printf(" %c %lld\n", hp->data[i]->c, hp->data[i]->frequence);
        }
      }
    }

    hp->size = t;

    build_max_heap(hp);
    heapsort(hp);

    hp->size = t;

    puts("");

    printf("\n print da heap depois do primeiro heapsort:\n\n");
    for(i=1;i<=t;i++) // print da heap depois de ordenadas as frequencias
    {
      printf(" %c %lld\n", hp->data[i]->c, hp->data[i]->frequence);
    }

    huff = building_huff_tree(hp, hp->size); // monta a arvore na heap
    free(hp); // liberando a memoria da heap ja que nao vamos precisar mais dela
    return huff;
  }

btree* Huffman(FILE *input, unsigned *freq, unsigned *tam)
{
  frequencia(input,freq); //funcao para achar a frequencia

  int i;
  puts("");
  printf(" Print do array inicial\n Caracteres e suas respectivas frequencias:\n\n");
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
  tam[0] = 0;
  print_pre(huff, tam);//printa em pre ordem e encontra o tamanho da arvore

  puts("");
  printf("\n Tamanho da arvore: %d\n",tam[0]);
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
  long long int key_index = hp->size;
  long long int parent_index = get_parent_index(hp, hp->size);
  while(parent_index >= 1 && hp->data[key_index]->frequence < hp->data[parent_index]->frequence)
  {
    swap(hp->data[key_index], hp->data[parent_index]);
    key_index = parent_index;
    parent_index = get_parent_index(hp, key_index);
  }
}

void max_heapify(heap *hp, long long int i)
{
  long long int biggest;
  long long int left_index = left_index_(hp,i);
  long long int right_index= right_index_(hp,i);

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
  long long int i;
  for(i = hp->size; i>=2; i--)
  {
    swap(hp->data[1], hp->data[i]);

    hp->size--;

    max_heapify(hp,1);
  }
}

void build_max_heap(heap *hp)
{
  long long int i;
  long long int size = hp->size;
  for(i=size/2;i>=1;i--)
  {
    max_heapify(hp,i);
  }
}

long long int get_parent_index(heap *hp,long long int i) { return (i/2); }
long long int left_index_(heap *hp,long long int i) { return (2*i); }
long long int right_index_(heap *hp,long long int i) { return (2*i + 1); }

void swap(btree *a, btree *b)
{
  btree aux = *a;
  *a = *b;
  *b = aux;
}

void frequencia(FILE *input, unsigned *freq)
{
  byte c;
  while(fscanf(input, "%c", &c) != EOF)
  {
    if(c != NULL) 
    {
      freq[c] += 1;
    }
    else

      freq[0] += 1;
  }

  rewind(input); // volta o arquivo ao seu comeco
}

void print_pre(btree *tree, unsigned *tam)
{
  if(tree != NULL)
  {
    tam[0]++;
    printf("%c",tree->c);
    print_pre(tree->left, tam);
    print_pre(tree->right, tam);
  }
}

byte* add_l(byte *binary, long long int *i)
{
  binary[i[0]] = '0';
  i[0]++;
  return binary;
}

byte* add_r(byte *binary, long long int *i)
{
  binary[i[0]] = '1';
  i[0]++;
  return binary;
}

void put(hash *hs, byte indice, byte *binary)
{
  element *new_element = (element*) malloc(sizeof(element));
  strcpy(new_element->binary, binary);
  hs->table[indice] = new_element;
}

void Hash(hash *hs, btree *arv, byte *binary, long long int *i)
{
  if(arv != NULL)
  {
    if(arv->left == NULL && arv->right == NULL)
    {
      put(hs, arv->c, binary);
      binary[i[0]] = NULL;
      i[0]--;
      return;
    }
    binary = add_l(binary,i);
    Hash(hs, arv->left, binary, i);
    binary = add_r(binary,i);
    Hash(hs, arv->right, binary, i);
    binary[i[0]] = NULL;
    i[0]--;
  }
}

void print_dictionary(hash *hs)
{
  for(int i = 0;i <= 255;i++)
  {
    if(hs->table[i] != NULL)
    {
      printf(" %c\n ",i);
      puts(hs->table[i]->binary);
    }
  }
}

byte set_bit(byte c, long long int i)
{
  /*
    Essa funcao eh usada para setar um bit 0 em 1, por exemplo:
    O byte 00110010
    Quero setar a ultima posicao dele como 1
    Entao eu dou i = 0
    Mask = 1 << (7 - 0) = 1 << 7
    Mask vai vir disso -> 00000001 para isso -> 10000000
    E vai fazer o 'ou' bit-a-bit com o byte que vc mandou para setar
    Vai ser:
    00110010
    Ou
    10000000
    ________
    10110010 <- aqui esta o byte com o bit que vc queria setar, retorna ele
  */
  byte mask = 1 << (7 - i);
  return mask | c;
}

void make_header(byte *bits, byte one[0], byte two[0])
{
  /*
      Usa a string com as informacoes do lixo e do tamanho da arvore
      e associa cada '1' da string ao respectivo bit no short de 2 bytes
      PS: a string deve ser do mesmo tamanho do short // 16 bits // 16 caracteres // string[16]
  */
  int i;
  int aux = 0;
  for(i = 0;i < 16; i++)
  {
    if(i < 8)
    {
      if(bits[i] == '1')
      {
        one[0] = set_bit(one[0], aux);
      }
    }
    else
    {
      if(bits[i] == '1')
      {
        two[0] = set_bit(two[0], aux);
      }
    }
    aux++;
    if(aux == 8)
    {
      aux = 0;
    }
  }
}

byte *Dec_to_binary(long long int n, byte *binary, long long int size)
{
  /*
      Essa funcao transforma um numero long long inteiro em binario e adiciona ele
      na string principal onde contem as informacoes de lixo e tam da arvore
  */
  long long int i = 0;
  long long int k = (size - 1);
  binary[k] = NULL;
  while (n > 0)
  {
    if(n % 2 == 1)
    {
      binary[k] = '1';
    }
    else
    {
      binary[k] = '0';
    }
    n = n / 2;
    k--;
  }
  while(k >= 0)
  {
    binary[k] = '0';
    k--;
  }
  return binary;
}

byte *make_pre_order(btree *huff, long long int *cont, byte *pre_order)
{
  /*
      Essa funcao usa a arvore de huffman para fazer a string com os caracteres da pre ordem
      que irei usar para por no cabecalho do arquivo
      Ela tambem considera o caso de que uma folha pode ser '*' ou '\'
      que eh necessaria para usar o caracter de escape
      Por exemplo se a arvore tier um noh folha com o caracter '*'
      entao sera inserido na string um caracter de escape '\' que servira
      para indicar que o proximo caracter eh uma folha
      assim ficando com:
      Ex: **ABCD\*     <- esse \* indica que * eh uma folha.
      Isso sera necessario para montar a arvore na descompressao
  */

  if(huff != NULL)
  {
    if(huff->left == NULL && huff->right == NULL && (huff->c == '*' || huff->c == '\\'))
    {
      pre_order[cont[0]] = '\\';
      cont[0]++;
    }
    pre_order[cont[0]] = huff->c;
    cont[0]++;
    make_pre_order(huff->left, cont, pre_order);
    make_pre_order(huff->right, cont, pre_order);
  }
  return pre_order;
}

FILE *construct_file(FILE *input, hash *dictionary, byte *pre_order, byte *bits, int size, byte c)
{
  /*
      deu ateh uma preguica explicar essa daqui ate porque olha o tanto de informacoes que tem por aqui,
      bom, primeiramente
      Eh criado um arquivo chamado output.txt (temporario favor mudar para .huff depois)
      que serah nosso arquivo comprimido
      em seguida eh declarado um tipo short (two_bytes) VAZIO que usarei ele para guarda o espaco de
      2 bytes no comeco do arquivo para depois modificalo com as informacoes
      logo apos, escrevo o pre-ordem no arquivo (escrevo os caracteres mesmo)
      Depois eu comeco a compressao do arquivo original para o comprimido
      pegando caracter por caracter e utilizando a string da posicao dele na hash para setar os bits do byte
      onde ele sera armazenado. Ao mesmo tempo verificando se o byte foi totalmente preenchido
      verifiquei nesse if(i==8) [soh escrevemos o byte no arquivo quando ele estiver cheio]
      Se ao final desse processo o i for maior que 0, indica que sobrou bit no ultimo byte
      usei o i para saber quantos bits sobraram, subtraindo ele de 8
      *recomendo nao tentarem entender as maracutaias e atribuicoes que faco*
      *elas sao necessaria para o codigo estar funcionando direitinho :)*
      Logo apos isso eu comeco a modificar o cabecalho com o a string que contem o lixo e o tamanho da arvore
      quando isso acontecer o arquivo estara comprimido e bunitu c:
      retornamos esse arquivo pra main;
  */

  FILE *output = fopen("output.huff","wb");//ATE AQUI
  byte item;
  long long int p, k;
  long long int i = 0; // usarei para saber onde setar o bit e quando escrevelo no arquivo output
  long long int trash;
  two_bytes header; // parte binaria do cabecalho (2 bytes)
  byte null;
  fwrite(&header, 2, 1, output); // escreve um unsigned short (2 bytes) para armazenar o num de lixo e o tam da arvore depois
  for(i = 0;i< size;i++)
  {
    if(pre_order[i] != NULL)
    {
      fwrite(&pre_order[i], 1, 1, output);
    }
    else
    {
      fwrite(&null, 1, 1, output);
    }
  }
  //fwrite(pre_order, strlen(pre_order), 1, output); // escreve a pre-ordem da arvore
  i = 0;
  while(fscanf(input, "%c", &item) != EOF)
  {
    if(item != NULL)
    {
      p = strlen(dictionary->table[item]->binary);
      for(k = 0 ; k < p ; k++ )
      {
        if(dictionary->table[item]->binary[k] == '1')
        {
          //             posicao em binario:(76543210)
          c = set_bit(c, i); // torna o bit em 1 Ex: 00000000 / usando i = 0, ele vai fazer 7 - 0 e dar 1<<7 / 10000000
          i++;
        }
        if(dictionary->table[item]->binary[k] == '0')
        {
          i++;
        }
        if(i == 8) // chegou no final do byte
        {
          fwrite(&c, 1, 1, output); // escreve o byte modificado na output
          c = c << 8; // zera o byte
          i = 0;
        }
      }
    }
    else
    {
      p = strlen(dictionary->table[0]->binary);
      for(k = 0 ; k < p ; k++ )
      {
        if(dictionary->table[0]->binary[k] == '1')
        {
          //             posicao em binario:(76543210)
          c = set_bit(c, i); // torna o bit em 1 Ex: 00000000 / usando i = 0, ele vai fazer 7 - 0 e dar 1<<7 / 10000000
          i++;
        }
        if(dictionary->table[0]->binary[k] == '0')
        {
          i++;
        }
        if(i == 8) // chegou no final do byte
        {
          fwrite(&c, 1, 1, output); // escreve o byte modificado na output
          c = c << 8; // zera o byte
          i = 0;
        }
      }
    }
  }
  if( i > 0 ) // sobrou lixo no byte
  {
    fwrite(&c, 1, 1, output); // mesmo sobrando lixo temos que escrever o byte na output
    trash = 8 - i; // quantidade de bits que ficaram de lixo
    printf(" Lixo: %lld\n",trash);
    strcpy(bits, Dec_to_binary(trash, bits, 3));
  }
  puts("\n Lixo \\ Tamanho da arvore\n ");
  bits[16] = NULL; // gambiarra
  printf(" ");
  puts(bits);
  printf("\n Tamanho dos 2 bytes:\n %ld\n",strlen(bits));
  puts("");

  rewind(input); // volta o arquivo de input para o comeco (por precaucao)

  // aqui comeca a mudanca no cabecalho
  byte one;
  byte two;
  make_header(bits, &one, &two);

  puts("\n Deus eh bom\n");

  puts("\n");
  rewind(output); // para poder atualizar o cabecalho com o lixo e o tamanho da arvore
  printf("\n\n     ARQUIVO COMPRIMIDO !!!\n");
  fwrite(&one, 1, 1, output); // atualizando o cabecalho
  fwrite(&two, 1, 1, output); // atualizando o cabecalho
  puts("");
  return output;
}

int is_bit_i_set(byte c, long long int i)
{
  byte mask = 1 << i;
  return mask & c;
}

btree *tree_from_pre_order(btree *huff, byte *pre_order, int *cont)
{
  if(cont[0] == 0)
  {
    huff = create_btree(pre_order[cont[0]], 0, NULL, NULL);
    cont[0]++;
    huff->left = tree_from_pre_order(huff->left, pre_order, cont);
    huff->right = tree_from_pre_order(huff->right, pre_order, cont);
  }
  else
  {
    if(pre_order[cont[0]] == '\\')
    {
      huff = create_btree(pre_order[cont[0] + 1], 0, NULL, NULL);
      cont[0] += 2;
      return huff;
    }
    else
    {
      if(pre_order[cont[0]] != '*')
      {
        huff = create_btree(pre_order[cont[0]], 0, NULL, NULL);
        cont[0]++;
        return huff;
      }
      else
      {
        huff = create_btree(pre_order[cont[0]], 0, NULL, NULL);
        cont[0]++;
      }
    }
    huff->left = tree_from_pre_order(huff->left, pre_order, cont);
    huff->right = tree_from_pre_order(huff->right, pre_order, cont);
  }
  return huff;
}

FILE *construct_file_decompress(FILE *input, btree *huff,int tam_cabecalho, int trash)
{
  FILE *output = fopen("descompressed.txt","wb");
  btree *aux = huff;
  int i,cont = 0;
  byte c;
  byte byte_with_trash;
  byte null;
  long int last_byte;
  fseek(input, -1, SEEK_END);
  last_byte = ftell(input);
  fread(&byte_with_trash, 1,1,input); // so pra printar
  printf("\n posicao do byte com o lixo no arquivo: %ld\n byte com o lixo: %d\n\n",last_byte, byte_with_trash);
  fseek(input, tam_cabecalho , SEEK_SET);
  cont = tam_cabecalho;
  while(fread(&c, 1, 1, input))
  {
    //printf("%c\n",c);
    if(cont == last_byte)
    {
      for(i = 7; i >= trash;i--)
      {
        if(is_bit_i_set(c,i))
        {
          aux = aux->right;
        }
        else
        {
          aux = aux->left;
        }
        if(aux->left == NULL && aux->right == NULL)
        {
          fwrite(&aux->c, 1, 1, output);
          aux = huff;
        }
      } 
    }
    else
    {
      for(i = 7;i >= 0;i--)
      {
        if(is_bit_i_set(c, i))
        {
          aux = aux->right;
        }
        else
        {
          aux = aux->left;

        }
        if(aux->left == NULL && aux->right == NULL)
        {
          fwrite(&aux->c, 1, 1, output);
          aux = huff;
        }
      }   
    }    
    cont++;
  }
  return output;
}

FILE *Decompress(FILE *input, btree *huff)
{
  FILE *output;
  byte c;
  byte one,two;
  fread(&one, 1, 1, input);
  fread(&two, 1, 1, input);
  long long int i = 0;
  int trash = 0;
  int tree_size = 0;
  int cont = 0;
  int aux = 0;
  int file_start;
  byte pre_order[1000];
  for(i=0;i<16;i++) // pega o tamanho do lixo e arvore no cabecalho
  {
    if(i<8)
    {
      if(is_bit_i_set(two,i))
      {
        tree_size += pow(2,i);
      }
    }
    else
    {
      if(i<13)
      {
        if(is_bit_i_set(one,aux))
        {
          tree_size += pow(2,i);
        }
        aux++;
      }
      else
      {
        if(is_bit_i_set(one,aux))
        {
          trash += pow(2,cont);
        }
        cont++;
        aux++;
      }
    }
  }
  cont = 0;
  aux = tree_size;
  printf("aux = %d\n",aux);
  while(aux)
  {
    fscanf(input,"%c",&c);
    pre_order[cont] = c;
    cont++;
    aux--;
  }
  cont = 0;
  printf("\n Array do pre_order:\n ");
  puts(pre_order);
  printf("%ld\n",strlen(pre_order));
  file_start = tree_size + 2;
  printf(" tree: %d  trash: %d bytes do arq: %d\n\n ",tree_size, trash, file_start);
  huff = tree_from_pre_order(huff, pre_order, &cont);
  unsigned tam = 0; // soh pra funcao print funcionar
  puts("");
  printf("\n Arvore de Huffman do arquivo:\n\n ");
  print_pre(huff,&tam);
  puts("");
  output = construct_file_decompress(input, huff, file_start, trash);
  free(huff);
  printf("\n\n\t DEUS EH CONOSCO IRMOES!!\n\n\n\n\t Arquivo Descompactado!!!\n\n ");
  return output;
}

int main()
{
  system("color 0A");
  FILE *input;
  FILE *output;
  btree *huff;
  hash *dictionary;
  byte arq[100]; // variavel para guardar o nome ou diretorio do arquivo (ex: arquivo.txt)
  printf("\n Insira o nome.tipo do arquivo para executar uma acao:\n\n ");

  scanf("%s",arq); //Ex: C:\Users\USER_NAME\Documents\CAPCOM\aloha.txt

  puts("");
  input = fopen(arq, "rb"); // fopen "chama" o arquivo, fopen("arquivo.tipo", "forma") forma- r,w,a (rb, wb, ab) binario
  //                                                  (r+, w+, a+) criacao
  if(input == NULL) // verifica se o arquivo eh valido
  {
    printf("\n O arquivo digitado nao existe ou nao foi encontrado.\n");
    exit(1); // retornar 1 indica que o arquivo e invalido
  }
  printf(" [ c // d ]\n\n ");
  printf("Insira o modo:\n\n ");
  byte caminho[20]; // variavel que salva a escolha

  scanf("%s",caminho); // ainda falta arrumar, escolha para compactar ou descompactar

  if(!strcasecmp(caminho,"c"))
  {

    unsigned freq[257] = {0}; // array que guarda as frequencias dos caracteres
    unsigned tam;

    huff = Huffman(input,freq,&tam); // criar a arvore de huffman pelo arquivo inserido

    dictionary = create_dictionary(); // criar o dicionario
    
    byte binary[9999] = {0}; // array para salvar o binario de cada caracter
    long long int i = 0;

    Hash(dictionary, huff, binary, &i); // funcao que adiciona os binarios de cada caracter do dicionario da arvore de huffman

    printf("\n Dicionario (ordem de acordo com a tabela ascii):\n ");
    puts("");
    print_dictionary(dictionary); // printa o dicionario pela ordem da tabela ascii
    puts("");

    byte c;

    byte pre_order[99999] = {0};
    long long int cont = 0;
    strcpy(pre_order, make_pre_order(huff, &cont, pre_order));
    
    //puts(pre_order);
    //tam = strlen(pre_order);

    byte header[16]; // binario do cabecalho
    strcpy(header, Dec_to_binary(cont, header, 16));

    output = construct_file(input, dictionary, pre_order, header, cont, c); // gera um output.txt (logo serah .huff) na sua pasta
    free(huff);
    free(dictionary);
    fclose(output);

  }
  else if(!strcasecmp(caminho,"d"))
  {
    output = Decompress(input, huff);
    
    fclose(output);
    free(huff);
  }
  else
  {
    printf("\n\n Error!!\n\n Modo Invalido\n Verifique se digitou errado e tenta novamente\n\n ");
    fclose(input);
    exit(2); // retornar 2 indica que o modo selecionado eh invalido
  }

  fclose(input);

  return 0; // retornar 0 significa que a operacao foi bem sucedida
}
