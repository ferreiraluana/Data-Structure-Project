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
  long long int comp;
};

struct qheap
{
  item_of_queue *data;
  int capacity;
  int size;
  int comp;
};

qheap *create_qheap(qheap *heap, long long int capacity)
{
  qheap *curr = (qheap*)malloc(1*sizeof(qheap));
  curr->capacity = capacity;
  curr->size = 0;
  curr->data = (item_of_queue*)malloc((capacity + 1) * sizeof(item_of_queue));
  return curr;

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

int enqueue(qheap *hp, int value, int size_, int find)
{
    hp->comp = 0;
    hp->size++;
    hp->data[hp->size].value= value;
    int key_index = hp->size;
    int parentindex = parent_index(hp,hp->size);
    hp->comp +=1;
    if (hp->size > size_)
    {
      printf("Heap Overflow\n");
    }
    else
    {
      while (parentindex >= 1 && hp->data[key_index].value > hp->data[parentindex].value)
      {
        hp->comp +=1;
        swap(&hp->data[key_index],&hp->data[parentindex]); ////////////////tinha &
        key_index = parentindex;
        parentindex = parent_index(hp,key_index);
      }
    }

    if(find == value)
      printf("Number of iterations: %d\n", hp->comp);

    return (hp->comp);
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

FILE *plot1(FILE *f, int item)
{
  f = fopen("plotFILE.csv", "a+");
  fprintf(f, "%d", item);
  return f;
}

int main()
{
  int size_,num,quantity;
  FILE *plot;

  printf("Pick a size for your heap: \n");
  scanf("%d", &size_);
  qheap *new_qheap = create_qheap(new_qheap, size_);

  int i, chosen;
  printf("WHAT NUMBER ARE YOU LOOKING FOR?\n");
  scanf("%d", &chosen);

  srand(time(NULL));
  for (i = 1; i <= size_; i++)
  {
    num = rand();
    quantity = enqueue(new_qheap, num, size_, chosen);
    plot = plot1(plot,i);
    plot = plot1(plot,quantity);
    plot = plot1(plot,i); ///ajeitar p colocar \n
  }
  fclose(plot);

  return 0;
}
