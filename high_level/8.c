#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 1000

// Проверка правильности расставленных скобок ({[]})

void main()
{
  char c;
  char str[MAX_LEN] = {0};
  FILE *in;
  in = fopen("C:/Users/Maximich/Documents/c_labs/high_level/input.txt", "r");
  if (in == NULL)
  {
    perror("nonexistent output file");
  }
  while ((c = fgetc(in)) != EOF)
  {
    if (c == '(' || c == '{' || c == '[')
      str[strlen(str)] = c;
    else if (str[strlen(str) - 1] == '(' && c == ')')
      str[strlen(str) - 1] = 0;
    else if (str[strlen(str) - 1] == '{' && c == '}')
      str[strlen(str) - 1] = 0;
    else if (str[strlen(str) - 1] == '[' && c == ']')
      str[strlen(str) - 1] = 0;
    else if (c == ')' || c == '}' || c == ']') {
      printf("Parentheses are mismatched");
      exit(0);
    }
  }
  if (!strlen(str))
    printf("Parentheses are correct");
  else
    printf("Parentheses are mismatched");
}