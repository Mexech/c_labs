#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 1000
int seq_end = 0;

int getnum() {
  int i = 0;
  char str[MAX_LEN] = {0};
  if (!seq_end)
    while ((*(str + i++) = getchar()) != ' ' && *(str + i - 1) != '\n');
  if (*(str + i - 1) == '\n')
    seq_end = 1;
  return atoi(str);
}

void main()
{
  int j, i = 0;
  int a[MAX_LEN] = {0};
  char str[MAX_LEN] = {0};
  FILE *out;
  out = fopen("C:/Users/Maximich/Documents/c_labs/high_level/output.txt", "w");
  if (out == NULL)
  {
      perror("nonexistent output file");
  }
  while ((*(a + i++) = getnum()) != 0);
  for (j = 0; j < i - 1; j++)
  {
    memset(str, 0, MAX_LEN);
    if (a[j] < 0)
      a[j] = 0;
    memset(str, 46, a[j]);
    str[a[j]] = '\n';
    fputs(str, out);
  }
}