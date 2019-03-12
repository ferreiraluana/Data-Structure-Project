#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct heap heap;

heap *create_heap(int capacity);

void enqueue(heap * hp, int item);

int parent_index(heap *hp, int item);
int left_index(heap *hp, int item);
int right_index(heap *hp, int item);

bool is_heap_empty(heap *hp);

void heapify(heap *hp, int i);
void heapsort(heap *hp);

int dequeue(heap *hp);
void destroy(heap *hp);

int main()
{
  heap *new_heap = create_heap(10);

  int num;
  char choice;

  int i;
  printf("Enqueue list:\n");
  for(i = 0; i < 10; i++)
  {
  	printf("|%d|", i);
  	enqueue(new_heap, i);
  }printf("\n");

  printf("Dequeue list:\n");
  for(i = 0; i < 10; i++)
  {
  	printf("|%d|", dequeue(new_heap));
  }

  destroy(new_heap);
  return 0;
}

struct heap
{
  int size;
  int capacity;
  int *data;
};

heap *create_heap(int capacity)
{
  heap *current = (heap*)malloc(1 * sizeof(heap));
  current->size = 0;
  current->capacity = capacity;
  current->data = (int*)malloc((capacity + 1) * sizeof(int));
  return (current);
}

void swap(int *a, int *b)
{
  int aux = *a;
  *a = *b;
  *b = aux;
}

void enqueue(heap *hp, int item)
{
  if (hp->size >= hp->capacity) {printf("Heap Overflow\n");}
  else
  {
    hp->data[++ hp->size] = item;

    int key_index = hp->size;
    int parentindex = parent_index(hp, hp->size);

    while (parentindex >= 1 && hp->data[key_index] > hp->data[parentindex])
    {
      swap(&hp->data[key_index], &hp->data[parentindex]);
      key_index = parentindex;
      parentindex = parent_index(hp, key_index);
    }
  }
}

int dequeue(heap *hp)
{
  if (is_heap_empty(hp))
  {
    printf("Heap Underflow\n");
    return (INT_MIN);
  }
  else
  {
    int item = hp->data[1];
    hp->data[1] = hp->data[hp->size];
    hp->size--;

    heapify(hp,1);
    return(item);
  }
}

int parent_index(heap *hp, int i){return (i >> 1);}
int left_index(heap *hp, int i){return (i << 1);}
int right_index(heap *hp, int i){return ((i << 1) + 1);}

bool is_heap_empty(heap *hp)
{
  return (!hp->size);
}

void heapify(heap *hp, int i)
{
  int largest;
  int left_index_ = left_index(hp,i);
  int right_index_ = right_index(hp,i);

  if (left_index_ <= hp->size && hp->data[left_index_] > hp->data[i])
    largest = left_index_;
  else
    largest = i;

  if (right_index_ <= hp->size && hp->data[right_index_] > hp->data[largest])
    largest = right_index_;

  if (hp->data[i] != hp->data[largest])
  {
    swap(&hp->data[i], &hp->data[largest]);
    heapify(hp, largest);
  }
}

void heapsort(heap *hp)
{
  int i;
  for (i = hp->size; i >= 2; i--)
  {
    swap(&hp->data[1], &hp->data[i]);

    hp->size--;
    heapify(hp,1);
  }
}

void destroy(heap *hp){free(hp->data); free(hp);}
