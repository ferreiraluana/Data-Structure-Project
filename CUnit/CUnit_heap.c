#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#define MAX_SIZE 257
//gcc -o CUnit_heap CUnit_heap.c -lm -lcunit
//./CUnit_heap
typedef unsigned char byte;

typedef struct btree
{
  long long int frequence;
  byte c;
  struct btree *left;
  struct btree *right;
}btree;

typedef struct heap
{
  long long int size;
  btree *data[MAX_SIZE];
} heap;

heap* create_heap();
btree* create_btree(byte c, long long int freq, btree *left, btree *right);
btree *dequeue(heap *hp);
void enqueue(heap *hp, btree *tree);
void min_heapify(heap *hp, long long int i);
void heapsort(heap *hp);
void build_min_heap(heap *hp);
void swap(btree *a, btree *b);
long long int get_parent_index(heap *hp,long long int i);
long long int left_index_(heap *hp,long long int i);
long long int right_index_(heap *hp,long long int i);

long long int get_parent_index(heap *hp,long long int i) { return (i/2); }
long long int left_index_(heap *hp,long long int i) { return (2*i); }
long long int right_index_(heap *hp,long long int i) { return (2*i + 1); }

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

heap* create_heap()
{
  heap *new_heap = (heap*) malloc(sizeof(heap));
  new_heap->size = 0;
  int i;
  for(i=1;i<=256;i++)
  {
    new_heap->data[i] = NULL;
  }
  return new_heap;
}

void swap(btree *a, btree *b)
{
  btree aux = *a;
  *a = *b;
  *b = aux;
}

btree *dequeue(heap *hp) // remove o item da heap
{
  btree *aux = hp->data[1];
  hp->data[1] = hp->data[hp->size];
  hp->size--;

  min_heapify(hp, 1);

  return aux;
}

void enqueue(heap *hp, btree *tree)
{
  hp->size+=1;
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

void heapsort(heap *hp)
{
  long long int i;
  for(i = hp->size; i >= 1; i--)
  {
    swap(hp->data[1], hp->data[i]);

    hp->size--;

    min_heapify(hp,1);
  }
}

void build_min_heap(heap *hp)
{
  long long int i;
  long long int size = hp->size;
  for(i = size/2 ;i >= 1;i--)
  {
    min_heapify(hp,i);
  }
}

void test_create_heap()
{
  heap *h = create_heap();
  CU_ASSERT(h != NULL);
} 

void test_create_btree()
{
  btree *tree;// = create_btree('a', 1, NULL, NULL);
  CU_ASSERT(tree != NULL);
} 

void test_enqueue()
{
  heap *hp = create_heap();
  btree *tree = create_btree('a', 1, NULL, NULL);
  enqueue(hp, tree);
  CU_ASSERT(hp->data[hp->size]->frequence == 1);
  CU_ASSERT(hp->data[hp->size]->c == 'a');
}

int main()
{
    // Initialize the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();
   // Sets the basic run mode, CU_BRM_VERBOSE will show maximum output of run details
   // Other choices are: CU_BRM_SILENT and CU_BRM_NORMAL
  CU_basic_set_mode(CU_BRM_VERBOSE);
   // Run the tests and show the run summary

  
  CU_pSuite pSuite = NULL;

// Add a suite to the registry
  pSuite = CU_add_suite("TESTES", 0, 0);
// Always check if add was successful
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
// Add the test to the suite
  if (NULL == CU_add_test(pSuite, "create_heap - test 1", test_create_heap)) 
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(pSuite, "create_btree - test 2", test_create_btree)) 
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

   if (NULL == CU_add_test(pSuite, "enqueue - test 3", test_enqueue)) 
  {
    CU_cleanup_registry();
    return CU_get_error();
  }


  CU_basic_run_tests();
  return CU_get_error();
}
