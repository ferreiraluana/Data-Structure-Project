#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define RED 1
#define BLACK 0
typedef struct binary_tree binary_tree;
//Left Leaning Red Black Tree

struct binary_tree {
    int item;
    int color;
    binary_tree *left;
    binary_tree *right;
};

binary_tree* create_empty_binary_tree();// Cria uma arvore vazia(NULL)

void color_swap(binary_tree *bt); // Inverte a cor do pai e de seus filhos(nao altera o conteudo ou a estrutura da arvore)

int color(binary_tree *bt);//acessa a cor da arvore

//complexidade das rotacoes O(1)
binary_tree* rotate_left(binary_tree *bt);//recebe a arvore BT, seu filho direito se torna seu pai e recebe sua cor, e a arvore BT se torna o filho esquerdo e fica RED

binary_tree* rotate_right(binary_tree *bt);//recebe a arvore BT, seu filho esquerdo se torna seu pai e recebe sua cor, e a arvore BT se torna o filho direito e fica RED 

binary_tree* move_left_RED(binary_tree *bt);//move um node RED para a esquerda

binary_tree* move_right_RED(binary_tree *bt);//move um node RED para a direita

binary_tree* balance_factor(binary_tree *bt);//arruma o balaceamento da arvore

binary_tree* add_bt(binary_tree *bt, int value);//inserindo...

binary_tree* add_arv(binary_tree *bt, int value);//responsavel pela busca do local de insercao

int search(binary_tree *bt, int value, int *flag);//procura para saber se o item a ser removido esta na arvore

binary_tree* minor_search(binary_tree *bt);//procura o menor

binary_tree* minor_remove(binary_tree *bt);//remove o menor

binary_tree* remove_bt(binary_tree *bt, int value);//remove

binary_tree* remove_arv(binary_tree *bt, int value);//responsavel pela busca do node a ser removido

void print_pre_order(binary_tree *bt);


int main()
{
    binary_tree *arv = create_empty_binary_tree(); 
    int i, N = 7,  x, val[7] ={20,15,30,85,50,21,70};

    for(i=0; i< N; i++){        
        arv = add_arv(arv,val[i]);
    }
    arv = remove_arv(arv,70);
    arv = remove_arv(arv,10);
    print_pre_order(arv);
    return 0;
}

binary_tree* create_empty_binary_tree()
{
    return NULL;
}

void color_swap(binary_tree *bt)
{
    bt->color = !bt->color;
    if(bt->left != NULL) bt->left->color = !bt->left->color;
    
    if(bt->right != NULL) bt->right->color = !bt->right->color;
}

int color(binary_tree *bt)
{
    if(bt == NULL) return BLACK;
    else return bt->color;
}

binary_tree* rotate_left(binary_tree *bt)
{
    binary_tree *aux = bt->right;
    bt->right = aux->left;
    aux->left = bt;
    aux->color = bt->color;
    bt->color = RED;
    return aux;
}

binary_tree* rotate_right(binary_tree *bt)
{
    binary_tree *aux = bt->left;
    bt->left = aux->right;
    aux->right = bt;
    aux->color = bt->color;
    bt->color = RED;
    return aux;
}

binary_tree* move_left_RED(binary_tree *bt)
{
    color_swap(bt);//recebe uma arvore e inverte as cores dele e de seus filhos
    if(color(bt->right->left) == RED) // se filho esquerdo do filho direito eh RED
    {
        bt->right = rotate_right(bt->right); // rotaciona o filho direito para direita
        bt = rotate_left(bt);//rotaciona a arvore para esquerda
        color_swap(bt);//inverte as cores
    }
    return bt;
}

binary_tree* move_right_RED(binary_tree *bt)
{
    color_swap(bt);
    if(color(bt->left->left) == RED)//se o filho esquerdo do filho esquerdo eh RED
    {
        bt = rotate_right(bt);//rotaciona a arvore para direita
        color_swap(bt);//inverte as cores
    }
    return bt;
}

binary_tree* balance_factor(binary_tree *bt)
{
    if(color(bt->right) == RED) bt = rotate_left(bt);//se o filho direito eh RED, rotaciona para esquerda
    
    if(bt->left != NULL && color(bt->right) == RED && color(bt->left->left) == RED) bt = rotate_right(bt);//se o filho direito � RED e o filho esquerdo do filho esquerdo � RED
                                                                                                            //rotaciona para direita
    if(color(bt->left) == RED && color(bt->right) == RED) color_swap(bt);//se os filhos sao RED, inverte as cores do pai e dos filhos 
    
    return bt;
}

binary_tree* add_bt(binary_tree *bt, int value)
{
    if(bt == NULL)
    {
        binary_tree *new_binary_tree = (binary_tree*)malloc(sizeof(binary_tree));
        if(new_binary_tree == NULL)//AINDA TO PENSANDO SE ISSO EH UTIL
        {
            return NULL;
        }
        new_binary_tree->item = value;//   NADA MAIS EH QUE A
        new_binary_tree->color = RED;//    create_binary_tree
        new_binary_tree->left = NULL;
        new_binary_tree->right = NULL;
        //*flag = 1;
        return new_binary_tree;
    }
    
    if(value != bt->item){
        if(value < bt->item) bt->left = add_bt(bt->left, value);
        else if(value > bt->item) bt->right = add_bt(bt->right, value);
    }
    
    if(color(bt->right) == RED && color(bt->left) == BLACK) bt = rotate_left(bt);
    
    if(color(bt->left) == RED && color(bt->left->left) == RED) bt = rotate_right(bt);
    
    if(color(bt->left) == RED && color(bt->right) == RED) color_swap(bt);
    
    return bt;
}

binary_tree* add_arv(binary_tree* bt, int value)
{
    bt = add_bt(bt, value);
    if(bt != NULL) bt->color = BLACK;
                                    
    return bt;
}

int search(binary_tree *bt, int value, int *flag)
{
    if(bt != NULL)
    {
        if(bt->item == value)
        {
            *flag = 1;
        }
        search(bt->left, value, flag);
        search(bt->right, value, flag);
    }

    return *flag;
}

binary_tree* minor_search(binary_tree *bt)
{
    binary_tree *a = bt;
    binary_tree *b = bt->left;
    while(b != NULL)
    {
        a = b;
        b = b->left;
    }
    return a;
}

binary_tree* minor_remove(binary_tree *bt)
{
    if(bt->left == NULL)
    {
        free(bt);
        return NULL;
    }
    if(color(bt->left) == BLACK && color(bt->left->left) == BLACK) bt = move_left_RED(bt);
    
    bt->left = minor_remove(bt->left);
    
    return balance_factor(bt);
}

binary_tree* remove_bt(binary_tree *bt, int value)
{
        if(value < bt->item)
        {
            if(color(bt->left) == BLACK && color(bt->left->left) == BLACK) bt = move_left_RED(bt);
        
            bt->left = remove_bt(bt->left, value);
        }
        else
        {
            if(color(bt->left) == RED) bt = rotate_right(bt);
        
            if(value == bt->item && bt->right == NULL)
            {
                free(bt);
                return NULL;
            }
            if(color(bt->right) == BLACK && color(bt->right->left) == BLACK) bt = move_right_RED(bt);
    
            if(value == bt->item)
            {
                binary_tree *aux = minor_search(bt->right);
                bt->item = aux->item;
                bt->right = minor_remove(bt->right);
            }else bt->right = remove_bt(bt->right, value);
        }
        return balance_factor(bt);
        
}

binary_tree* remove_arv(binary_tree *bt, int value)
{
    int flag = 0;
    if(search(bt, value, &flag))
    {
        binary_tree *aux = bt;
        bt = remove_bt(aux, value);
        if(bt != NULL) bt->color = BLACK;
    }
    return bt;
}

void print_pre_order(binary_tree *bt)
{
    if (bt!=NULL) {
    printf("Value:%d  Color:%d\n", bt->item,bt->color);
    print_pre_order(bt->left);
    print_pre_order(bt->right);
    }
}