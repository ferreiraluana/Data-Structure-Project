
byte set_bit(byte c, long long int i)
{
  byte mask = 1 << (7 - i);
  return mask | c;
}

byte *Dec_to_binary(long long int n, byte *binary, long long int size)
{
  long long int i = 0;
  long long int k = (size - 1);
  binary[k] = NULL;
  while (n > 0)
  {
    if(n % 2 == 1)
    {
      binary[k] = '1';
    }
    else
    {
      binary[k] = '0';
    }
    n = n / 2;
    k--;
  }
  while(k >= 0)
  {
    binary[k] = '0';
    k--;
  }
  return binary;
}

byte *make_pre_order(btree *huff, long long int *cont, byte *pre_order)
{
  if(huff != NULL)
  {
    if(huff->left == NULL && huff->right == NULL && (*((byte*)huff->c) == '*' || *((byte*)huff->c) == '\\'))
    {
      pre_order[cont[0]] = '\\';
      cont[0]++;
    }
    pre_order[cont[0]] = *((byte*)huff->c);
    cont[0]++;
    make_pre_order(huff->left, cont, pre_order);
    make_pre_order(huff->right, cont, pre_order);
  }
  return pre_order;
}

void frequence(FILE *input, unsigned int *freq) //////////////////////void pointer 1
{
  byte c;
  while(fscanf(input, "%c", &c) != EOF)
  {
    if(c != NULL)
    {
      freq[c] += 1;
    }
    else

      freq[0] += 1;
  }

  rewind(input); // volta o arquivo ao seu comeco
}

void make_header(byte *header_string, byte *one, byte *two)
{
  int i;
  int aux = 0;
  for(i = 0;i < 16; i++)
  {
    if(i < 8)
    {
      if(header_string[i] == '1')
      {
        one[0] = set_bit(one[0], aux);
      }
    }
    else
    {
      if(header_string[i] == '1')
      {
        two[0] = set_bit(two[0], aux);
      }
    }
    aux++;
    if(aux == 8)
    {
      aux = 0;
    }
  }
}

FILE *construct_file(FILE *input, hash *dictionary, byte *pre_order, byte *header_string, int size, byte c) //// talvez tenha q mudar p void aqui tb
{
  FILE *output = fopen("output.huff","wb");

  long long int p, k;
  long long int i = 0; // usarei para saber onde setar o bit e quando escrevelo no arquivo output
  long long int trash;

  two_bytes header_space; // parte binaria do cabecalho (2 bytes)
  byte null;
  fwrite(&header_space, 2, 1, output); // escreve um unsigned short (2 bytes) para armazenar o num de lixo e o tam da arvore depois

  for(i = 0;i < size;i++)
  {
    if(pre_order[i] != NULL)
    {
      fwrite(&pre_order[i], 1, 1, output);
    }
    else
    {
      fwrite(&null, 1, 1, output);
    }
  }

  byte item;
  i = 0;
  while(fscanf(input, "%c", &item) != EOF)
  {
    if(item != NULL)
    {
      p = strlen(dictionary->table[item]->binary);
      for(k = 0 ; k < p ; k++)
      {
        if(dictionary->table[item]->binary[k] == '1')
        {
          c = set_bit(c, i);
          i++;
        }
        if(dictionary->table[item]->binary[k] == '0')
        {
          i++;
        }
        if(i == 8) // chegou no final do byte
        {
          fwrite(&c, 1, 1, output); // escreve o byte modificado na output
          c = c << 8; // zera o byte
          i = 0;
        }
      }
    }
    else
    {
      p = strlen(dictionary->table[0]->binary);
      for(k = 0 ; k < p ; k++ )
      {
        if(dictionary->table[0]->binary[k] == '1')
        {
          //             posicao em binario:(76543210)
          c = set_bit(c, i); // torna o bit em 1 Ex: 00000000 / usando i = 0, ele vai fazer 7 - 0 e dar 1<<7 / 10000000
          i++;
        }
        if(dictionary->table[0]->binary[k] == '0')
        {
          i++;
        }
        if(i == 8) // chegou no final do byte
        {
          fwrite(&c, 1, 1, output); // escreve o byte modificado na output
          c = c << 8; // zera o byte
          i = 0;
        }
      }
    }
  }
  if( i > 0 ) // sobrou lixo no byte
  {
    fwrite(&c, 1, 1, output); // mesmo sobrando lixo temos que escrever o byte na output
    trash = 8 - i; // quantidade de bits que ficaram de lixo
    printf(" Lixo: %lld\n",trash);
    strcpy(header_string, Dec_to_binary(trash, header_string, 3));
  }
  puts("\n Lixo \\ Tamanho da arvore\n ");
  header_string[16] = NULL;
  printf(" ");
  puts(header_string);
  puts("");

  byte one;
  byte two;
  make_header(header_string, &one, &two);

  rewind(output); // para poder atualizar o cabecalho com o lixo e o tamanho da arvore

  fwrite(&one, 1, 1, output); // atualizando o cabecalho
  fwrite(&two, 1, 1, output); // atualizando o cabecalho

  printf("\n\n     ARQUIVO COMPRIMIDO !!!\n");
  puts("");
  return output;
}