#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 1000
int seq_end = 0;

int is_delimiter(int c)
{
  return c == ' ' || c == EOF || c == '\n' || c == '\r' || c == 0;
}

// Вывести содержимое файла в виде таблицы(типо с разделителями, но любыми и видимо размеры вводятся с клавы)
void main()
{
  int j, i = 0, t = 0;
  int x;
  char str[MAX_LEN] = {0};
  FILE *in, *out;
  in = fopen("C:/Users/Maximich/Documents/c_labs/high_level/input.txt", "r");
  if (in == NULL)
  {
      perror("nonexistent output file");
  }
  out = fopen("C:/Users/Maximich/Documents/c_labs/high_level/output.txt", "w");
  if (out == NULL)
  {
      perror("nonexistent output file");
  }
  scanf("%d", &x);
  int max = 0;
  str[j] = fgetc(in);
  do {
    if (is_delimiter(str[j]))
    {
      if (j > max)
        max = j;
        j = -1;
      memset(str, 0, MAX_LEN);
    }
  } while((str[++j] = fgetc(in)) != EOF && j < MAX_LEN);
  rewind(in);
  j = 0;
  str[j] = fgetc(in);
  char spaces[MAX_LEN] = {0};
  do {
    if (is_delimiter(str[j]))
    {
      if (strlen(str) > 1)
        i++;
      str[j] = 0;
      memset(spaces, 32, max - strlen(str));
      strcat(str, spaces);
      memset(spaces, 0, MAX_LEN);
      if (i == x) {
        strcat(str, "\n");
        i = 0;
      }
      else
        strcat(str, " ");
      fprintf(out, str);
      j = -1;
      memset(str, 0, MAX_LEN);
    }
  } while((str[++j] = fgetc(in)) != EOF && j < MAX_LEN);
}