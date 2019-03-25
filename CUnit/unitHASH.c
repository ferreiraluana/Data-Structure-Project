#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_header.h"

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

void put_string_in_hash(hash *ht, byte index, byte *binary)
{
  element *new_element = (element*) malloc(sizeof(element));
  strcpy(new_element->binary, binary);
  ht->table[index)] = new_element;
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



void test_create_dictionary()
{
  hash *new_hash = create_dictionary();
  CU_ASSERT(new_hash != NULL);
}

void test_put_in_hash()
{
  hash *hs = create_dictionary();
  put_string_in_hash(hs, 'x', "teste");
  CU_ASSERT(strcmp(hs->table['x'], "teste"));
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
  if (NULL == CU_add_test(pSuite, "create_dictionary - test 1", test_create_dictionary))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (NULL == CU_add_test(pSuite, "put_in_hash - test 2", test_put_in_hash))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }


  CU_basic_run_tests();
  return CU_get_error();
}
