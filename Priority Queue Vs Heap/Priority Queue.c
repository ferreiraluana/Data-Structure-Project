#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

typedef struct priority_queue p_q;
typedef struct node list;

p_q *create_priority_queue();
list *create_node(int value, int priority);

bool is_empty(p_q *pq);

list *front(p_q *pq);
int max(p_q *pq);
void print_priority_queue(p_q *pq, char mode);
void enqueue(p_q *pq, int value, int priority);

void dequeue(p_q *pq);
void destroy(p_q *pq);

int main()
{
  p_q *new_pq = create_priority_queue();
  printf("%s\n", is_empty(pq) == true ? "true" : false);
  char op1;
  int op2,op3,op4;
  scanf("%c", &op1);
  do
  {
  switch (op1)
  {
    case 'e': printf("Insira o valor desejado:\n"); scanf("%d", &op2);
    printf("Insira a prioridade desse valor:\n"); scanf("%d", &op3);
    enqueue(new_pq,op2,op3); break;
    case 'd': printf("Insira o valor a ser removido:\n"); scanf("%d", &op4);
    break;
    case 'm': printf("Max: %d\n", max(new_pq)); break;
    case 'p': print_priority_queue(new_pq); break;
  }
  }while(op1 != 'e');

  destroy(new_pq);
  return 0;
}

struct priority_queue
{
  list *head;
};

struct node
{
  int value;
  int priority;
  list *next;
};

p_q *create_priority_queue()
{
  p_q *current = (p_q*)malloc(1 * sizeof(p_q));
  current->head = NULL;
  return (current);
}

list *create_node(int value, int priority)
{
  list *current = (list*)malloc(1 * sizeof(list));
  current->value = value;
  current->priority = priority;
  current->next = NULL;
  return (current);
}

bool is_empty(p_q *pq){return (pq->head == NULL);}

list *front(p_q *pq){return (pq->head);}

int max(p_q *pq){return(pq->head->value);}

void print_priority_queue(p_q *pq, char mode)
{
  list *current = pq->head;
  while(current->next != NULL)
  {
    printf("%d %c\n", current->value, mode);
    current = current->next;
  } printf("%d\n", current->value);
}

void enqueue(p_q *pq, int value, int priority)
{
  list *current = create_node(value,priority);
  if (is_empty(pq) || current->priority > pq->head->priority)
  {
    current->next = pq->head;
    pq->head = current;
  }
  else
  {
    list *current_l  = pq->head;
    while(current_l->next != NULL && current->priority < current_l->next->priority)
    {
      current_l = current_l->next;
    }
    current->next = current_l->next;
    current_l->next = current;
  }
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
