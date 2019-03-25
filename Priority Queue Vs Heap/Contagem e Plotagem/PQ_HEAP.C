/*PRIORITY QUEUE WITH HEAP*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct heap qheap;
typedef struct item_of_queue item;

qheap *create_qheap(int capacity);

void enqueue(qheap * hp, int value, int priority,int chosen);

int parent_index(qheap *hp, int value);
int left_index(qheap *hp, int value);
int right_index(qheap *hp, int value);
void swap(item *a, item *b);
bool is_qheap_empty(qheap *hp);

void max_qheapify(qheap *hp, int i);
void qheapsearch(qheap *hp,int num);

int dequeue(qheap *hp);
//void destroy(heap *hp);

int main()
{
  printf("Pick you heap size: \n");
  int size_; scanf("%d", &size_);
  qheap *new_qheap = create_qheap(size_);
  srand(time(NULL));
  int chosen,i,num,num2; //scanf("%d", &chosen);
  for (i = 1; i <= size_; i++)
  {
  	num = rand();
  	num2= rand();// se quiser que as prioridades fiquem todas doidas, deixa assim, mas se quiser ordenado, mas aleatório, tira o num2 e coloca num,num
    enqueue(new_qheap,num,num2,chosen);
  }//printf("%d", new_qheap->comparisons);
  for (i = 1; i <= size_; i++)
  {
    printf("|%d|", dequeue(new_qheap));
  }

  return 0;
}

struct heap
{
  item *data;
  int capacity;
  int size;
  int comparisons;
  int priority;
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
  curr->comparisons = 0;
  curr->data = (item*)malloc((capacity+1) * sizeof(item));
  return curr;
}

void enqueue(qheap *hp, int value, int priority,int chosen)
{
  hp->comparisons = 0;
	hp->data[++hp->size].priority = priority;
    hp->data[hp->size].value= value;
    int key_index = hp->size;
    int parentindex = parent_index(hp,hp->size);
    hp->comparisons++;

    while (parentindex >= 1 && hp->data[key_index].priority > hp->data[parentindex].priority)
    {
      hp->comparisons++;
      swap(&hp->data[key_index],&hp->data[parentindex]);
      key_index = parentindex;
      parentindex = parent_index(hp,key_index);
    }
    if (hp->size != hp->capacity)
    {
      FILE *output;
      output = fopen("pq_with_heap.csv", "a+");
      fprintf(output, "%d,%d\n", hp->size, hp->comparisons);
      fclose(output);
    }
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
