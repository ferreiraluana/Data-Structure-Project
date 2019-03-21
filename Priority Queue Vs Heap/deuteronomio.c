#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

typedef struct heap qheap;
typedef struct item_of_queue item;

qheap *create_qheap(int capacity);
int parent_index(qheap *hp, int value);
int left_index(qheap *hp, int value);
int right_index(qheap *hp, int value);
int dequeue(qheap *hp);
bool is_qheap_empty(qheap *hp);
void swap(item *a, item *b);
void enqueue(qheap *hp, int value, int priority);
void max_qheapify(qheap *hp, int i);
void heapsearch(qheap *hp, int start, int end, int chosen, int *comparisons);

struct heap
{
  item *data;
  int capacity;
  int size;
  //int priority;
};

struct item_of_queue
{
  int value;
  int priority;
};

qheap *create_qheap(int capacity)
{
  qheap *curr = (qheap*)malloc(1*sizeof(qheap));
  curr->capacity = capacity;
  curr->size = 0;
  curr->data = (item*)malloc((capacity) * sizeof(item));
  return curr;
}

bool is_qheap_empty(qheap *hp){return (!hp->size);}
int parent_index(qheap *hp, int i){return (i>>1);}
int left_index(qheap *hp, int i){return (i<<1);}
int right_index(qheap *hp, int i){return ((i<<1) + 1);}

void enqueue(qheap *hp, int value, int priority)
{ 
 hp->data[++hp->size].priority = priority;
 hp->data[hp->size].value = value;
 int key_index = hp->size;
 int parentindex = parent_index(hp,hp->size);

 if (is_qheap_empty(hp))
 {
  printf("Heap Overflow\n"); // ???
}
else
{
  while (parentindex >= 1 && hp->data[key_index].priority > hp->data[parentindex].priority)
  {
    swap(&hp->data[key_index],&hp->data[parentindex]);
    key_index = parentindex;
    parentindex = parent_index(hp,key_index);
  }
}   
}

void swap(item *a, item *b)
{
  item aux = *a;
  *a = *b;
  *b = aux;
}

void max_qheapify(qheap *hp, int i)
{
  int largest;
  int leftindex = left_index(hp,i);
  int rightindex = right_index(hp,i);

  if (leftindex <= hp->size && hp->data[leftindex].priority < hp->data[i].priority)
  {
    largest = leftindex;
  }
  else{largest = i;}

  if (rightindex <= hp->size && hp->data[rightindex].priority < hp->data[largest].priority)
  {
    largest = rightindex;
  }

  if (hp->data[i].priority != hp->data[largest].priority)
  {
    swap(&hp->data[i], &hp->data[largest]);
    max_qheapify(hp,largest);
  }
}

int dequeue(qheap *hp)
{
  if (hp->size == 0) {printf("Heap Underflow\n"); return 0;}
  int num = hp->data[1].value;
  hp->data[1] = hp->data[hp->size];
  hp->size--;
  max_qheapify(hp,1);
  return num;
}

void build_max_heap(qheap *heap)
{
  int i;
  for(i = (heap->size)/2; i >= 1; i--)
    max_qheapify(heap, i);
}

void heapsort(qheap *heap)
{
  build_max_heap(heap);
  int i;
  for(i = heap->size; i >=1; i--)
  {
    swap(&heap->data[1], &heap->data[i]);
    heap->size--;
    max_qheapify(heap, 1);
    //print(heap);
  }
}

void heapsearch(qheap *hp, int start, int end, int chosen, int *comparisons)
{
  *comparisons +=1;
  if(end >= 1)
  {
    //*comparisons += 1;
    int middle = (start + end)/2;
    if(hp->data[middle].priority == chosen) 
    {
      return ;
    }
    if(hp->data[middle].priority > chosen) 
    {
      heapsearch(hp, start, middle-1, chosen, comparisons);
    }
    if(hp->data[middle].priority < chosen) 
    {
      heapsearch(hp, middle + 1, end, chosen, comparisons);
    }
  }
  else
  {
    printf("NOT FOUND\n");
    return;
  }
} 

void main()
{
  int size; printf("TYPE THE HEAP MAXIMUM SIZE:\n");
  scanf("%d", &size); printf("\n");
  qheap *heap = create_qheap(size); // size + 1
  int i = 1, j, k, chosen, comparisons = 0;

  while(size--)
  {
    enqueue(heap, i, i); i++;
    build_max_heap(heap);
  }
  heapsort(heap);
  heap->size = i-1;

  printf("ENTER %d NUMBERS:\n", i-1);

  for(k = 1; k < i; k++)
  {
    //scanf("%d", &chosen);
    printf("it = %d prio = %d\n", heap->data[k].value, heap->data[k].priority);
   // printf("%d comparisons\n", comparisons);
    //comparisons = 0;
  }

  printf("\n");
  for (j = 1; j < i; j++)
  {
    printf("|%d|", dequeue(heap));
  } printf("\n");
}
