
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