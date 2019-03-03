#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          RED-BLACK TREE                                              //
// In the Red-Black Tree we have black and red nodes and some rules to be followed;                     //
//  1. You must create one more bit of space for the node color in the main struct;                     //
//  2. A red node ALWAYS have two black children;                                                       //
//  3. Every leaf that is NULL are considered BLACK;                                                    //
//  4. The root is BLACK;                                                                               //
//  5. All the paths from the node to descendant leaves contains ALWAYS the same number of black nodes; //
//////////////////////////////////////////////////////////////////////////////////////////////////////////


typedef struct red_black_tree rbt;

rbt *create_empty_bstree(void);

bool is_empty(rbt *avp);

rbt *create_bstree((*void) item, int color, rbt *left, rbt *right, rbt *parent);

rbt* add_rbt(rbt* avp,(*void) item);

rbt *insert(rbt *avp, void *item);

rbt *check(rbt *avp, void *item);

void main()
{
	rbt *new_rbt = create_empty_bstree();

	char op,op1;
	char op2,op3,op4;

	do
	{
		scanf("%c", &op1); op = getchar();

		switch(op1)
			{
				case 'i': scanf("%", &op3)/*Não sei como faz referência para void para scanear a variável, se alguem descobrir corrige aqui.*/; 
				new_rbt = insert(new_rbt, op3)//inserir
				case 'p': //printar
				case 'c': //checkar/consultar
			}
	} while(op1 != 'e');
}

struct red_black_tree
{
	void *item;
	int color;
	red_black_tree *left;
	red_black_tree *right;
	red_black_tree *parent;
};

rbt *create_empty_bstree(void){return NULL;}

bool is_empty(rbt *avp){return (avp == NULL);}

rbt *create_bstree(void *item, int color, rbt *left, rbt *right, rbt *parent)
{
	rbt *temp = (rbt*)malloc(1 * sizeof(rbt));
	temp->item = item;
	temp->color = color;
	temp->left = left;
	temp->right = right;
	temp->parent = parent;
	return (temp);
}

rbt *insert(rbt *avp, void *item)
{
	rbt *temp = (rbt*)malloc(1 * sizeof(rbt));

	//if (is_empty(temp)){puts("\aErro de memória. Saindo do programa..."); exit(1);}
	
	if (check(avp,item) == -1)//se a árvore tiver algum nó vazio,adiciona;
	{
		temp->item = item; temp->left = NULL; temp->right = NULL; temp->color = 1; /*a cor de adição é sempre vermelha,a variável temporaria
		recebe o item dado a função, já left e right ficam nulos;*/

		rbt *x = avp; //cria-se uma variável temporária que serve como "auxiliar" com os valores de avp;
		rbt *y = NULL; //isso sim é uma variavel auxiliar;

		while(!is_empty(x)) //se o lugar que x aponta que recebeu avp não estiver vazia,procuramos o seu lugar/nó na árvore
		{
			y = x; //serve como um auxiliar que compara as ponteiros/arvores
			if (temp->item < x->item){x = x->left;}
			else{x = x->right;}
		} temp->parent = y;

		if (is_empty(y)){avp = z;} //
		else
		{
			if (temp->item < y->item){y->left = temp;}
			else{y->right = temp;}
		}

		//avp = rotation_insert();
	}
	return (avp);
}

rbt *check(rbt *avp, void *item)// função básica que percorre a árvore em log(n)
{
	if (avp == NULL){return (-1);} //se estiver vazia retorna que não encontrou,simples
	else
	{
		if (item == avp->item){return 1;} //caso tenha encontrado
		else
		{
			if (item > avp->item){return check(avp->left,item);} /* se o item da função tiver valor maior que o item da struct, vasculha 
			a esquerda numa recursão*/
			else{return check(avp->right,item);} // se não for maior, vasculha a direita.
		}
	}
}
