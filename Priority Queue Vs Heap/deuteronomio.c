#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX 100000
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
  qheap *current = (qheap*)malloc(1*sizeof(qheap));
  current->capacity = capacity;
  current->size = 0;
  current->data = (item*)malloc((capacity + 1) * sizeof(item));
  return current;
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

  if (leftindex <= hp->size && hp->data[leftindex].priority > hp->data[i].priority)
  {
    largest = leftindex;
  }
  else{largest = i;}

  if (rightindex <= hp->size && hp->data[rightindex].priority > hp->data[largest].priority)
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

void heapsearch(qheap *hp, int start, int end, int chosen, int *comparisons)
{
  *comparisons +=1;
  if(end >= 1)
  {
    int middle = (start + end)/2;
    if(hp->data[middle].priority == chosen) 
    {
      return;
    }
    else if(hp->data[middle].priority > chosen) 
    {
      heapsearch(hp, start, middle-1, chosen, comparisons);
    }
    else if(hp->data[middle].priority < chosen) 
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
  int i, j, k, chosen, comparisons = 1, size1;
  int item, preference;
  srand(time(NULL));
  size1 = size;
  while(size--)
  {
    item = rand() % MAX;
    preference = item;
    //preference = rand() % MAX;
    enqueue(heap, item, preference); 
  }
  printf("HOW MANY NUMBERS DO YOU WANNA FIND?\n");
  scanf("%d", &i); printf("\n");
  printf("ENTER THE NUMBERS TO BE FOUND:\n");

  for(k = 1; k <= i; k++)
  {
    scanf("%d", &chosen);
    heapsearch(heap, 1, size, chosen, &comparisons);
    printf("%d,%d\n", chosen, comparisons);
    //printf("|%d|->%d\n", heap->data[k].value, heap->data[k].priority);
    comparisons = 0;
  }printf("\n");
  
  for (j = 1; j < size1; j++)
  {
    printf("|%d|", dequeue(heap));
  } printf("\n");
  free(heap);
}
