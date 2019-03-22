#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
//botei a binary search do jeito que vi no slide e que lembrava e saiu isso
typedef struct qheap qheap;
typedef struct item_of_queue item_of_queue;

struct item_of_queue
{
  long long int value;
};

struct qheap
{
  item_of_queue *data;
  int capacity;
  int size;
};

qheap *create_qheap(qheap *heap, long long int capacity)
{
  qheap *curr = (qheap*)malloc(1*sizeof(qheap));
  curr->capacity = capacity;
  curr->size = 0;
  curr->data = (item_of_queue*)malloc((capacity + 1) * sizeof(item_of_queue));
  return curr;
///////////////////////////////////////

  // heap = (qheap*) malloc(sizeof(qheap));
  // heap->capacity = capacity;
  // heap->size = 0;
  // heap->comparations = 0;
  // heap->data = (item_of_queue*) realloc((capacity+1) * sizeof(item_of_queue));

  //return heap;

}

int parent_index(qheap *hp, int i){return (i>>1);}
int left_index(qheap *hp, int i){return (i<<1);}
int right_index(qheap *hp, int i){return ((i<<1) + 1);}

void swap(item_of_queue *a, item_of_queue *b)
{
  item_of_queue aux = *a;
  *a = *b;
  *b = aux;
}

void enqueue(qheap *hp, int value, int size_, int *comp)
{
    *comp+=1;
    hp->size++;
    hp->data[hp->size].value= value;
    int key_index = hp->size;
    int parentindex = parent_index(hp,hp->size);

    if (hp->size > size_)
    {
      printf("Heap Overflow\n");
    }
    else
    {
      while (parentindex >= 1 && hp->data[key_index].value > hp->data[parentindex].value)
      {
        *comp+=1;
        swap(&hp->data[key_index],&hp->data[parentindex]); ////////////////tinha &
        key_index = parentindex;
        parentindex = parent_index(hp,key_index);
      }
    }
}

bool is_qheap_empty(qheap *hp){ return (!hp->size);}

void max_qheapify(qheap *hp, int i, int *comp)
{
  (*comp)+=1;
  int largest;
  int leftindex = left_index(hp,i);
  int rightindex = right_index(hp,i);

  if (leftindex <= hp->size && hp->data[leftindex].value > hp->data[i].value)
  {
    largest = leftindex;
  }
  else{largest = i;}

  if (rightindex <= hp->size && hp->data[rightindex].value > hp->data[largest].value)
  {
    largest = rightindex;
  }

  if (hp->data[i].value != hp->data[largest].value)
  {
    swap(&hp->data[i], &hp->data[largest]);
    max_qheapify(hp,largest,comp); ///ver esse &(*comp)
  }
}

int main()
{
  int size_, comparison = 0,num;
  printf("Pick a size for your heap: \n");
  scanf("%d", &size_);
  qheap *new_qheap = create_qheap(new_qheap, size_);

  int i, chosen;
  srand(time(NULL));
  for (i = 1; i <= size_; i++)
  {
    num = rand();
    enqueue(new_qheap,num,size_,&comparison);
    printf("enqueue of num === %d comparisons\n", i, comparison);
    comparison = 0;
  }

  printf("Heap: \n");
  for (i = 1; i <= size_; i++)
  {
    printf(" %d\n", new_qheap->data[i].value);
  }printf("\n");

  return 0;
}
