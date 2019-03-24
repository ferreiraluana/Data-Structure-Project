/*PRIORITY QUEUE*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct priority_queue p_q;
typedef struct node list;

p_q *create_priority_queue(int runs);
list *create_node(int value, int priority);
bool is_empty(p_q *pq);
void print_priority_queue(p_q *pq);
p_q *enqueue(p_q *pq, int value, int priority);
void dequeue(p_q *pq);
void destroy(p_q *pq);

int main(void)
{
  int runs;
  printf("Pick your queue size: ");
  scanf("%d", &runs);
  p_q *pq = create_priority_queue(runs);
  printf("Creation: %s\n", is_empty(pq) == true ? "true" : false);
  int priority,i = 0;
  srand(time(NULL));

    int j,chosen,cpriority,counter = 0;
    int num,num2;
    for (i = 1; i <= runs; i++)
    {
      num2 = rand();
      num = rand();
      enqueue(pq,num,num2);
    }
  print_priority_queue(pq);
  destroy(pq);
  return 0;
}

struct priority_queue
{
  list *head;
  int max_size;
  int comparisons;
  int size;
};

struct node
{
  int value;
  int priority;
  list *next;
};

p_q *create_priority_queue(int runs)
{
  p_q *current = (p_q*)malloc(1 * sizeof(p_q));
  current->head = NULL;
  current->comparisons = 0;
  current->size = 0;
  current->max_size = runs;
  return (current);
}

bool is_empty(p_q *pq){return (pq->head == NULL);}

list *front(p_q *pq){return (pq->head);}

int max(p_q *pq){return(pq->head->value);}


p_q *enqueue(p_q *pq, int value, int priority)
{
  list *current = (list*)malloc(sizeof(list));
  current->value = value;
  current->priority = priority;
  if (is_empty(pq) || current->priority > pq->head->priority)
  {
    if (!is_empty(pq)){pq->comparisons++;}
    //current->comparisons++;
    current->next = pq->head;
    pq->head = current;
  }
  else
  {
    list *current_l  = pq->head;
    while(current_l->next != NULL && current->priority < current_l->next->priority)
    {
    	pq->comparisons++;
      current_l = current_l->next;
    }
    current->next = current_l->next;
    current_l->next = current;
  }
  pq->size++;
  if (pq->size != pq->max_size)
  {
    FILE *output;
    output = fopen("priority_queue.csv", "a");
    fprintf(output, "%d,%d\n", pq->size, pq->comparisons);
    fclose(output);
  }
  //current->comparisons++;
  //return comparisons;
}

void print_priority_queue(p_q *pq)
{
  list *current = pq->head;
  while (current->next != NULL)
  {
    printf("|%d|", current->value);
    current = current->next;
  } printf("|%d|\n", current->value);
}

void dequeue(p_q *pq)
{
  if (is_empty(pq)){return;}
  list *current = pq->head;
  pq->head = pq->head->next;
  free(current);
}

void destroy(p_q *pq)
{
  while(!is_empty(pq)){dequeue(pq);}
  free(pq);
}
