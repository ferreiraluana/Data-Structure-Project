#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

typedef struct red_black_tree rbt;

void main()
{
	char op,op1;
	char op2,op3,op4;

	scanf("%c", &op1); op = getchar();
	
	switch(op1)
	{
		case 'i': scanf("%d", &op3);//inserir
		case 'p': //printar
	}
}

struct red_black_tree
{
	(*void) item; //não tenho certeza se está correto;
	int color;//também não tenho certeza se aqui precisa ser um inteiro.
	red_black_tree *left;
	red_black_tree *right;
	red_black_tree *parent;
};
