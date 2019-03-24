
long long int get_parent_index(heap *hp,long long int i) { return (i/2); }
long long int left_index_(heap *hp,long long int i) { return (2*i); }
long long int right_index_(heap *hp,long long int i) { return (2*i + 1); }

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

btree *dequeue(heap *hp) // remove o item da heap
{
  btree *aux = hp->data[1];
  hp->data[1] = hp->data[hp->size];
  hp->size--;

  min_heapify(hp, 1);

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
  for(i = hp->size; i >= 2; i--)
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

void swap(btree *a, btree *b)
{
  btree aux = *a;
  *a = *b;
  *b = aux;
}
