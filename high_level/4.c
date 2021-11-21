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
  while ((*(a + i++) = getnum()) != 0);
  for (j = 0; j < i; j++)
  {
    if (a[j] < 0)
      a[j] = 0;
    if (j % 2 != 0 && j != i - 1) {
      int t = a[j];
      a[j] = a[j-1];
      a[j-1] = t;
    }
  }
  for (j = 0; j < i - 1; j++)
    printf("%d ", a[j]);
}