#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "code_header.h"
#include "heap.h"
#include "dictionary.h"
#include "huff_tree.h"
#include "compress.h"
#include "decompress.h"


/*
/////////////////////////////////
////                         ////
////                         ////
////          HUFFMAN        ////
////                         ////
////                         ////
/////////////////////////////////
*/

int main()
{
  FILE *input;
  FILE *output;
  btree *huff;
  hash *dictionary;
  byte arq[150]; // variavel para guardar o nome ou diretorio do arquivo (ex: arquivo.txt)
  printf("\n Insira o nome.tipo do arquivo para executar uma acao:\n\n ");

  scanf("%s",arq); //Ex: C:\Users\USER_NAME\Documents\CAPCOM\aloha.txt

  puts("");
  input = fopen(arq, "rb");

  if(input == NULL) // verifica se o arquivo eh valido
  {
    printf("\n O arquivo digitado nao existe ou nao foi encontrado.\n");
    exit(1); // retornar 1 indica que o arquivo e invalido
  }

  printf(" [ c // d ]\n\n ");
  printf("Insira o modo:\n\n ");

  byte caminho[20]; // variavel que salva a escolha

  scanf("%s",caminho); // ainda falta arrumar, escolha para compactar ou descompactar

  if(!strcasecmp(caminho,"c"))
  {

    unsigned int freq[257] = {0}; // array que guarda as frequencias dos caracteres

    huff = Huffman(input,freq); // criar a arvore de huffman pelo arquivo inserido
    dictionary = create_dictionary(); // criar o dicionario

    byte binary[20] = {0}; // string para salvar o binario de cada caracter
    long long int cont = 0;

    Hash(dictionary, huff, binary, &cont); // funcao que adiciona os binarios de cada caracter do dicionario da arvore de huffman
    print_dictionary(dictionary);
    byte c;

    byte pre_order[520] = {0};
    cont = 0;
    strcpy(pre_order, make_pre_order(huff, &cont, pre_order));
    byte header_string[16];
    strcpy(header_string, Dec_to_binary(cont, header_string, 16));
    output = construct_file(input, dictionary, pre_order, header_string, cont, c);
    free(huff);
    free(dictionary);
    fclose(output);

  }
  else if(!strcasecmp(caminho,"d"))
  {
    Decompress(input, huff);
  }
  else
  {
    printf("\n\n Error!!\n\n Modo Invalido\n Verifique se digitou errado e tenta novamente\n\n ");
    fclose(input);
    exit(2);
  }

  fclose(input);

  return 0;
}
