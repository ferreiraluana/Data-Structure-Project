
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

btree* Huffman(FILE *input, unsigned int *freq) /////////////////////////////////////////////////// void pointer 1
{
  int i;
  frequence(input,freq); //funcao para achar a frequencia
  puts("");

  btree *huff = create_huffman_tree(freq); // manda o array de frequencias para que seja construida a arvore

  puts("");

  printf("\n Arvore de Huffman em pre-ordem:\n\n ");
  print_in_pre_order(huff);//printa em pre ordem e encontra o tamanho da arvore
  puts("");

  return huff;
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