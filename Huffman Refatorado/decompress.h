
int is_bit_i_set(byte c, long long int i)
{
  byte mask = 1 << i;
  return mask & c;
}

FILE *construct_file_decompress(FILE *input, btree *huff,int size_header, int trash)
{
  FILE *output = fopen("descompressed.txt","wb");
  btree *aux = huff;
  int i,cont = 0;

  byte c;
  byte byte_with_trash;
  byte null;

  long int last_byte;

  fseek(input, -1, SEEK_END);

  last_byte = ftell(input);

  fread(&byte_with_trash, 1,1,input); // so pra printar
  printf("\n\n posicao do byte com o lixo no arquivo: %ld\n byte com o lixo(Valor na tabela ascii): %d\n\n",last_byte, byte_with_trash);

  fseek(input, size_header , SEEK_SET);
  cont = size_header;

  while(fread(&c, 1, 1, input))
  {
    if(cont == last_byte)
    {
      for(i = 7; i >= trash;i--)
      {
        if(is_bit_i_set(c,i))
        {
          aux = aux->right;
        }
        else
        {
          aux = aux->left;
        }
        if(aux->left == NULL && aux->right == NULL)
        {
          fwrite(&aux->c, 1, 1, output);
          aux = huff;
        }
      }
    }
    else
    {
      for(i = 7;i >= 0;i--)
      {
        if(is_bit_i_set(c, i))
        {
          aux = aux->right;
        }
        else
        {
          aux = aux->left;

        }
        if(aux->left == NULL && aux->right == NULL)
        {
          fwrite(&aux->c, 1, 1, output);
          aux = huff;
        }
      }
    }
    cont++;
  }
  return output;
}

btree *tree_from_pre_order(btree *huff, byte *pre_order, int *cont)
{
  if(cont[0] == 0)
  {
    huff = create_btree(pre_order[cont[0]], 0, NULL, NULL);
    cont[0]++;
    huff->left = tree_from_pre_order(huff->left, pre_order, cont);
    huff->right = tree_from_pre_order(huff->right, pre_order, cont);
  }
  else
  {
    if(pre_order[cont[0]] == '\\')
    {
      huff = create_btree(pre_order[cont[0] + 1], 0, NULL, NULL);
      cont[0] += 2;
      return huff;
    }
    else
    {
      if(pre_order[cont[0]] != '*')
      {
        huff = create_btree(pre_order[cont[0]], 0, NULL, NULL);
        cont[0]++;
        return huff;
      }
      else
      {
        huff = create_btree(pre_order[cont[0]], 0, NULL, NULL);
        cont[0]++;
      }
    }
    huff->left = tree_from_pre_order(huff->left, pre_order, cont);
    huff->right = tree_from_pre_order(huff->right, pre_order, cont);
  }
  return huff;
}

void Decompress(FILE *input, btree *huff)
{
  FILE *output;
  byte c; // usar para ler o arquivo
  byte one,two;

  fread(&one, 1, 1, input);
  fread(&two, 1, 1, input);

  int trash = 0;
  int tree_size = 0;

  int cont = 0;
  int aux = 0;
  int i = 0;

  int file_start;
  byte pre_order[520];

  for(i=0;i<16;i++) // pega o tamanho do lixo e arvore no cabecalho
  {
    if(i<8)
    {
      if(is_bit_i_set(two,i))
      {
        tree_size += pow(2,i);
      }
    }
    else
    {
      if(i<13)
      {
        if(is_bit_i_set(one,aux))
        {
          tree_size += pow(2,i);
        }
        aux++;
      }
      else
      {
        if(is_bit_i_set(one,aux))
        {
          trash += pow(2,cont);
        }
        cont++;
        aux++;
      }
    }
  }

  cont = 0;
  aux = tree_size;

  while(aux)
  {
    fscanf(input,"%c",&c);
    pre_order[cont] = c;
    cont++;
    aux--;
  }

  printf("\n Array do pre_order:\n ");
  puts(pre_order);
  printf("\n\n");
  file_start = tree_size + 2;
  printf(" tree: %d  trash: %d bytes do comeco do arq: %d\n\n ",tree_size, trash, file_start);

  cont = 0;
  huff = tree_from_pre_order(huff, pre_order, &cont);

  output = construct_file_decompress(input, huff, file_start, trash);
  free(huff);
  fclose(output);
  printf("\n\n\n\n\n\n\n\t Arquivo Descompactado!!!\n\n ");
}
