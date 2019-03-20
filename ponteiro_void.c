#include <stdio.h>

void main( )
{
    int x = 5;
    void *y = (void *) &x;//cast: converte o tipo a ser definido (float *) , (int *)
    int *z;

    z = (int *) y;//olha o cast ai dnv gente ;D
    
    printf("%d \n\n",*z);//printa 5(no caso, o valor de x)
    
    *z = 9;
    printf("%d \n\n",*z);//printa 9
}
