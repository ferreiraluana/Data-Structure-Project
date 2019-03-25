#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_header.h"
#include "heap.h"

//typedef struct btree btree;
//typedef unsigned char byte;
// struct btree
// {
//   long long int frequence;
//   byte c;
//   btree *left;
//   btree *right;
// };

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

void test_create_hufftree()
{
  unsigned int teste[4] = {4,3,5,2};
  btree *new_btree = create_huffman_tree(teste);
  CU_ASSERT(new_btree != NULL);
}

void test_create_btree()
{
  btree* new_btree = create_btree('t',2,NULL,NULL);
  CU_ASSERT(new_btree!= NULL);
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
  if (NULL == CU_add_test(pSuite, "create_btree - test 1", test_create_btree))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (NULL == CU_add_test(pSuite, "create_huffman_tree - test 2", test_create_hufftree))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }


  CU_basic_run_tests();
  return CU_get_error();
}
