#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 1000
int seq_end = 0;

// В файле переставить местами строки чет/нечет

void main()
{
  int j = 0, i = 0;
  int x, y;
  char str1[MAX_LEN] = {0};
  char str2[MAX_LEN] = {0};
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
  while (str1[strlen(str1) - 1] != EOF && str2[strlen(str1) - 1] != EOF)
  {
    while ((str1[j++] = fgetc(in)) != '\n' && str1[j - 1] != EOF);
    if (str1[j - 1] == EOF) {
      str1[j - 1] = 0;
      break;
    }
    j = 0;
    while ((str2[j++] = fgetc(in)) != '\n' && str2[j - 1] != EOF);
    if (str2[j - 1] == EOF)
      str2[j - 1] = 0;
    j = 0;
    fprintf(out, str2);
    fprintf(out, str1);
  }
  fprintf(out, str1);
}