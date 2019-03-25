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
  void *c; ///////////////////////////// item read from the input file
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
  byte binary[100]; /// strings from dictionary
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

// hash* create_dictionary(); // cria o dicionario
heap* create_heap(); // cria uma heap de btrees
btree* create_btree(void *c, long long int freq, btree *left, btree *right); // cria um nova arvore
btree *building_huff_tree(heap *hp, long long int heap_original_size); // usa a heap para montar a arvore
btree* create_huffman_tree(unsigned int *freq); // cria todas as arvores na heap (com respectivos caracteres e frequencias)
btree* Huffman(FILE *input, unsigned int *freq); // funcao base para a formacao da arvore (funcao estruturadora)
btree *dequeue(heap *hp); // remove o item da heap
void enqueue(heap *hp, btree *tree); // adiciona o item na heap
void min_heapify(heap *hp, long long int i); // funcao complementar da heapsort()
void heapsort(heap *hp); // ordenar a heap
void build_min_heap(heap *hp); // funcao complementar da heapsort()
long long int get_parent_index(heap *hp,long long int i); // funcao complementar da heapsort()
long long int left_index_(heap *hp,long long int i); // funcao complementar da heapsort()
long long int right_index_(heap *hp,long long int i); // funcao complementar da heapsort()
void swap(btree *a, btree *b); // troca duas arvores na heap
void frequence(FILE *input, unsigned int *freq); // elabora o array de frequencia cujo indices sao seus respectivos caracteres
void print_in_pre_order(btree *tree); // imprime a arvore de huffman em pre-ordem e soma o numero de nodes
// byte* add_left(byte *binary, long long int *i); // funcao complementar da Hash()
// byte* add_right(byte *binary, long long int *i); // funcao complementar da Hash()
// void put_string_in_hash(hash *ht, byte index, byte *binary); // adiciona o binario do caracter no dicionario
// void Hash(hash *ht, btree *huff, byte *binary, long long int *i); // funcao base da criacao do dicionario
// void print_dictionary(hash *ht); // printa o dicionario por ordem da tabela ASCII
// byte set_bit(byte c, long long int i); // seta o bit em uma determinada posicao do byte
// void make_header(byte *header_string, byte *one, byte *two); // monta os 2 bytes do cabecalho
// byte *Dec_to_binary(long long int n, byte *binary, long long int size); // transforma um inteiro em binario e coloca esse binario em uma string
// byte *make_pre_order(btree *huff, long long int *cont, byte *pre_order); // monta a pre-ordem da arvore considerando se a folha eh '*' ou '\'
// FILE *construct_file(FILE *input, hash *dictionary, byte *pre_order, byte *header_string, int size, byte c); // constroi o arquivo comprimido (COM CABECALHO)
// int is_bit_i_set(byte c, long long int i); // verifica se o bit da posicao i estah setado
// btree *tree_from_pre_order(btree *huff, byte *pre_order, int *cont); // monta a arvore pela pre-ordem dada no arquivo
// FILE *construct_file_decompress(FILE *input, btree *huff,int size_header, int trash); // funcao para construir o arquivo descompactado usando a arvore
// void Decompress(FILE *input, btree *huff); // funcao base para a descompressao e leitura do cabecalho

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

btree* create_btree(void *c, long long int freq, btree *left, btree *right)
{
  btree *new_btree;
  new_btree = (btree*) malloc(sizeof(btree));

  if(new_btree == NULL) return NULL;
  new_btree->c = malloc(sizeof(byte));
  *((byte*)new_btree->c) = *((byte*)c);
  new_btree->frequence = freq;
  new_btree->left = left;
  new_btree->right = right;
  return new_btree;
}

btree *building_huff_tree(heap *hp, long long int heap_original_size)
{
  int i;
  if(hp->size == 1) return hp->data[1];

  btree *left = dequeue(hp); // retira o primeiro menor
  heap_original_size--;
  build_min_heap(hp);
  heapsort(hp); // ordena para colocar o segundo menor
  hp->size = heap_original_size;

  btree *right = dequeue(hp); // retira o segundo menor
  heap_original_size--;

  long long int freq = (left->frequence + right->frequence);
  byte c = '*';
  btree *sum = create_btree(&c, freq, left, right);
  enqueue(hp, sum); // adiciona a soma dos dois primeiros devolta na heap
  heap_original_size++;
  build_min_heap(hp);
  heapsort(hp);

  hp->size = heap_original_size;
  building_huff_tree(hp, heap_original_size);
}

btree* create_huffman_tree(unsigned int *freq)
{
  unsigned int i;
  long long int heap_original_size = 0;
  btree *aux;
  heap *hp = create_heap();
  btree *huff= NULL;

    for(i = 0;i <= 255;i++) // adicionando as frequencias na heap
    {
      if(freq[i] != 0)
      {
        aux = create_btree(&i,freq[i],NULL,NULL);
        enqueue(hp,aux);
        heap_original_size++;
      }
    }


    build_min_heap(hp);
    heapsort(hp);
    hp->size = heap_original_size;

    puts("");

    huff = building_huff_tree(hp, hp->size); // monta a arvore a partir da heap
    free(hp); // liberando a memoria da heap ja que nao vamos precisar mais dela
    return huff;
  }

void frequence(FILE *input, unsigned int *freq) //////////////////////void pointer 1
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

btree* Huffman(FILE *input, unsigned int *freq) /////////////////////////////////////////////////// void pointer 1
{
  int i;
  frequence(input,freq); //funcao para achar a frequencia
  puts("");

  btree *huff = create_huffman_tree(freq); // manda o array de frequencias para que seja construida a arvore
  puts("");

  return huff;
}

btree *dequeue(heap *hp) // remove o item da heap
{
  btree *aux = hp->data[1];
  hp->data[1] = hp->data[hp->size];
  hp->size--;

  min_heapify(hp, 1);

  return aux;
}
//
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
//
void min_heapify(heap *hp, long long int i)
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
    min_heapify(hp,biggest);
  }
}
//
void heapsort(heap *hp)
{
  long long int i;
  for(i = hp->size; i >= 2; i--)
  {
    swap(hp->data[1], hp->data[i]);

    hp->size--;

    min_heapify(hp,1);
  }
}
//
void build_min_heap(heap *hp)
{
  long long int i;
  long long int size = hp->size;
  for(i = size/2 ;i >= 1;i--)
  {
    min_heapify(hp,i);
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

void print_in_pre_order(btree *tree)
{
  if(tree != NULL)
  {
    printf("%c",*((byte*)tree->c));
    print_in_pre_order(tree->left);
    print_in_pre_order(tree->right);
  }
}

byte* add_left(byte *binary, long long int *i)
{
  binary[i[0]] = '0';
  i[0]++;
  return binary;
}

byte* add_right(byte *binary, long long int *i)
{
  binary[i[0]] = '1';
  i[0]++;
  return binary;
}

void put_string_in_hash(hash *ht, void *index, byte *binary)
{
  element *new_element = (element*) malloc(sizeof(element));
  strcpy(new_element->binary, binary);
  ht->table[*((byte*)index)] = new_element;
}

void Hash(hash *ht, btree *huff, byte *binary, long long int *i)
{
  if(huff != NULL)
  {
    if(huff->left == NULL && huff->right == NULL)
    {
      put_string_in_hash(ht, huff->c, binary);

      binary[i[0]] = NULL;
      i[0]--;
      return;
    }

    binary = add_left(binary,i);
    Hash(ht, huff->left, binary, i);

    binary = add_right(binary,i);
    Hash(ht, huff->right, binary, i);

    binary[i[0]] = NULL;
    i[0]--;
  }
}

void print_dictionary(hash *ht)
{
  for(int i = 0;i <= 255;i++)
  {
    if(ht->table[i] != NULL)
    {
      printf(" %c\n ",i);
      puts(ht->table[i]->binary);
    }
  }
}

byte set_bit(byte c, long long int i)
{
  byte mask = 1 << (7 - i);
  return mask | c;
}

void make_header(byte *header_string, byte *one, byte *two)
{
  int i;
  int aux = 0;
  for(i = 0;i < 16; i++)
  {
    if(i < 8)
    {
      if(header_string[i] == '1')
      {
        one[0] = set_bit(one[0], aux);
      }
    }
    else
    {
      if(header_string[i] == '1')
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
//
byte *make_pre_order(btree *huff, long long int *cont, byte *pre_order)
{
  if(huff != NULL)
  {
    if(huff->left == NULL && huff->right == NULL && (*((byte*)huff->c) == '*' || *((byte*)huff->c) == '\\'))
    {
      pre_order[cont[0]] = '\\';
      cont[0]++;
    }
    pre_order[cont[0]] = *((byte*)huff->c);
    cont[0]++;
    make_pre_order(huff->left, cont, pre_order);
    make_pre_order(huff->right, cont, pre_order);
  }
  return pre_order;
}

FILE *construct_file(FILE *input, hash *dictionary, byte *pre_order, byte *header_string, int size, byte c) //// talvez tenha q mudar p void aqui tb
{
  FILE *output = fopen("output.huff","wb");

  long long int p, k;
  long long int i = 0; // usarei para saber onde setar o bit e quando escrevelo no arquivo output
  long long int trash;

  two_bytes header_space; // parte binaria do cabecalho (2 bytes)
  byte null;
  fwrite(&header_space, 2, 1, output); // escreve um unsigned short (2 bytes) para armazenar o num de lixo e o tam da arvore depois

  for(i = 0;i < size;i++)
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

  byte item;
  i = 0;
  while(fscanf(input, "%c", &item) != EOF)
  {
    if(item != NULL)
    {
      p = strlen(dictionary->table[item]->binary);
      for(k = 0 ; k < p ; k++)
      {
        if(dictionary->table[item]->binary[k] == '1')
        {
          c = set_bit(c, i);
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
    strcpy(header_string, Dec_to_binary(trash, header_string, 3));
  }
  header_string[16] = NULL;
  puts("");

  byte one;
  byte two;
  make_header(header_string, &one, &two);

  rewind(output); // para poder atualizar o cabecalho com o lixo e o tamanho da arvore

  fwrite(&one, 1, 1, output); // atualizando o cabecalho
  fwrite(&two, 1, 1, output); // atualizando o cabecalho

  printf("\n\n     ARQUIVO COMPRIMIDO !!!\n");
  puts("");
  return output;
}

int is_bit_i_set(byte c, long long int i)
{
  byte mask = 1 << i;
  return mask & c;
}

btree *tree_from_pre_order(btree *huff, byte *pre_order, int *cont) //// consertar isso da arvore
{
  void *c;
  if(cont[0] == 0)
  {
    c = &pre_order[cont[0]];
    huff = create_btree(c, 0, NULL, NULL); /// antes tava &pre_order[cont[0]]; //////////////ajeitar p aritmetica de ponteiro, p esse c,q eh void, receber essa posicao de byte de pre_ordere mandar na create q so aceita void no primeiro parametro
    cont[0]++;
    huff->left = tree_from_pre_order(huff->left, pre_order, cont);
    huff->right = tree_from_pre_order(huff->right, pre_order, cont);
  }
  else
  {
    if(pre_order[cont[0]] == '\\')
    {
      c = &pre_order[cont[0] + 1];
      huff = create_btree(c, 0, NULL, NULL); /// antes tava &pre_order[cont[0]+1]; //////////////ajeitar p aritmetica de ponteiro, p esse c,q eh void, receber essa posicao de byte de pre_ordere mandar na create q so aceita void no primeiro parametro
      cont[0] += 2;
      return huff;
    }
    else
    {
      if(pre_order[cont[0]] != '*')
      {
        c = &pre_order[cont[0]]; /// antes tava &pre_order[cont[0]]; //////////////ajeitar p aritmetica de ponteiro, p esse c,q eh void, receber essa posicao de byte de pre_ordere mandar na create q so aceita void no primeiro parametro
        huff = create_btree(c, 0, NULL, NULL);
        cont[0]++;
        return huff;
      }
      else
      {
        c = &pre_order[cont[0]]; /// antes tava &pre_order[cont[0]]; //////////////ajeitar p aritmetica de ponteiro, p esse c,q eh void, receber essa posicao de byte de pre_ordere mandar na create q so aceita void no primeiro parametro
        huff = create_btree(c, 0, NULL, NULL);
        cont[0]++;
      }
    }
    huff->left = tree_from_pre_order(huff->left, pre_order, cont);
    huff->right = tree_from_pre_order(huff->right, pre_order, cont);
  }
  return huff;
}

FILE *construct_file_decompress(FILE *input, btree *huff,int size_header, int trash)
{
  FILE *output = fopen("descompressed.mp4","wb");
  btree *aux = huff;
  int i,cont = 0;
  byte c1;

  byte c;
  byte byte_with_trash;
  byte null;

  long long int last_byte;

  fseek(input, -1, SEEK_END);

  last_byte = ftell(input);

  fread(&byte_with_trash, 1,1,input); // so pra printar

  fseek(input, size_header , SEEK_SET);
  cont = size_header;

  while(fread(&c, 1, 1, input))
  {
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
          c1 = *((byte*)aux->c);
          fwrite(&c1, 1, 1, output);
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
          c1 = *((byte*)aux->c);
          fwrite(&c1, 1, 1, output);
          aux = huff;
        }
      }
    }
    cont++;
  }
  return output;
}

void Decompress(FILE *input, btree *huff)
{
  FILE *output;
  byte c; // usar para ler o arquivo
  byte one,two;

  fread(&one, 1, 1, input);
  fread(&two, 1, 1, input);

  int trash = 0;
  int tree_size = 0;

  int cont = 0;
  int aux = 0;
  long long int i = 0;

  int file_start;
  byte pre_order[520]; ////// tava 520 ///////////////////rever, no 513 printou 1 char a mais

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
  while(aux--)
  {
    fscanf(input,"%c",&c);
    pre_order[cont] = c;
    cont++;
  }

  file_start = tree_size + 2;

  cont = 0;
  huff = tree_from_pre_order(huff, pre_order, &cont);
  unsigned tam = 0; // soh pra funcao print funcionar
  puts("");
  output = construct_file_decompress(input, huff, file_start, trash);
  free(huff);
  fclose(output);
  printf("\n\n\n\n\n\n\n\t Arquivo Descompactado!!!\n\n ");
}

int main()
{
  FILE *input;
  FILE *output;
  btree *huff;
  hash *dictionary;
  byte arq[150]; // variavel para guardar o nome ou diretorio do arquivo (ex: arquivo.txt)
  printf("\n Insira o nome.tipo do arquivo para executar uma acao:\n\n ");

  scanf("%s",arq); //Ex: C:\Users\USER_NAME\Documents\CAPCOM\aloha.txt

  puts("");
  input = fopen(arq, "rb");

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

    unsigned int freq[257] = {0}; // array que guarda as frequencias dos caracteres

    huff = Huffman(input,freq); // criar a arvore de huffman pelo arquivo inserido
    dictionary = create_dictionary(); // criar o dicionario

    byte binary[20] = {0}; // string para salvar o binario de cada caracter
    long long int cont = 0;

    Hash(dictionary, huff, binary, &cont); // funcao que adiciona os binarios de cada caracter do dicionario da arvore de huffman
    byte c;

    byte pre_order[520] = {0};
    cont = 0;
    strcpy(pre_order, make_pre_order(huff, &cont, pre_order));
    byte header_string[16];
    strcpy(header_string, Dec_to_binary(cont, header_string, 16));
    output = construct_file(input, dictionary, pre_order, header_string, cont, c);
    free(huff);
    free(dictionary);
    fclose(output);

  }
  else if(!strcasecmp(caminho,"d"))
  {
    Decompress(input, huff);
  }
  else
  {
    printf("\n\n Error!!\n\n Modo Invalido\n Verifique se digitou errado e tenta novamente\n\n ");
    fclose(input);
    exit(2);
  }

  fclose(input);

  return 0;
}
