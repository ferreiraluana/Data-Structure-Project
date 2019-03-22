#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
//botei a binary search do jeito que vi no slide e que lembrava e saiu isso
typedef struct heap qheap;
typedef struct item_of_queue item;

qheap *create_qheap(long long int capacity);

void enqueue(qheap * hp, int value, int priority, int size_);

int parent_index(qheap *hp, int value);
int left_index(qheap *hp, int value);
int right_index(qheap *hp, int value);
void swap(item *a, item *b);
bool is_qheap_empty(qheap *hp);

void max_qheapify(qheap *hp, int i);
int heapsearch(qheap *hp,long long int chosen,long long int size, long long int comparison);

int dequeue(qheap *hp);
//void destroy(heap *hp);

int main()
{
  long long int size_, comparison = 0; 
  printf("Pick a size for your heap: \n");
  scanf("%lld", &size_);
  qheap *new_qheap = create_qheap(size_);

  printf("Choose a number to search: \n");
  int chosen,i; scanf("%d", &chosen);
  for (i = 1; i <= size_; i++)
  {
    enqueue(new_qheap,i,i,size_);
  }
  printf("Heap: \n");
  for (i = 1; i <= size_; i++)
  {
    printf("|%d|", dequeue(new_qheap));
  }printf("\n");
  printf("Iterations: \n");
  printf("%lld\n", heapsearch(new_qheap,chosen,size_,comparison));

  return 0;
}

struct heap
{
  item *data;
  int capacity;
  int size;
  int comparations;
};

struct item_of_queue
{
  long long int value;
  long long int priority;
};

qheap *create_qheap(long long int capacity)
{
  qheap *curr = (qheap*)malloc(1*sizeof(qheap));
  curr->capacity = capacity;
  curr->size = 0;
  curr->comparations = 0;
  curr->data = (item*)malloc((capacity + 1) * sizeof(item));
  return curr;
}

void enqueue(qheap *hp, int value, int priority, int size_)
{
    hp->data[++hp->size].priority = priority;
    hp->data[hp->size].value= value;
    int key_index = hp->size;
    int parentindex = parent_index(hp,hp->size);
    hp->comparations++;

    if (hp->size > size_)
    {
      printf("Heap Overflow\n");
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

int heapsearch(qheap *hp, long long int chosen, long long int size, long long int comparisons)
{
  long long int begin = 0, end = size - 1;
  int middle;

  while(begin <= end)
  {
    middle = (begin + end)/2;
    if (hp->data[middle].value < chosen)
    {
      comparisons++;
      begin = middle + 1;
    }
    else if (hp->data[middle].value > chosen)
    {
      comparisons++;
      end = middle - 1;
    }
    else
    {
      comparisons++;
      return comparisons;
    }
  }
  return 0;
}

int parent_index(qheap *hp, int i){return (i>>1);}
int left_index(qheap *hp, int i){return (i<<1);}
int right_index(qheap *hp, int i){return ((i<<1) + 1);}

void swap(item *a, item *b)
{
  item aux = *a;
  *a = *b;
  *b = aux;
}

bool is_qheap_empty(qheap *hp){ return (!hp->size);}

int dequeue(qheap *hp)
{
  if (hp->size == 0) {printf("Heap Underflow\n"); return 0;}
  int num = hp->data[1].value;
  hp->data[1] = hp->data[hp->size];
  hp->size--;
  max_qheapify(hp,1);
  return num;
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
