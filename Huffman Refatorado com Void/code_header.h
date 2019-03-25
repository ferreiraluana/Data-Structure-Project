
#define MAX_SIZE 257

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