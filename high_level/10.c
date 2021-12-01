#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int is_delimiter(int c)
{
  return c == ' ' || c == '.' || c == ',' || c == '\r' || c == '\n';
}

// Вводится символ, которым заменяют разделители в файле
void main() {
  FILE *in, *out;
  char *replace_str;
  char replace_c;
  char c;
  scanf("%s", replace_str);
  if (!strcmp("tab", replace_str))
    replace_c = '\t';
  else if (!strcmp("enter", replace_str))
    replace_c = '\n';
  else
    replace_c = replace_str[0];
  in = fopen("F:/Projects/c_labs/high_level/input.txt", "r");
  if (in == NULL)
  {
    perror("nonexistent input file");
  }
  out = fopen("F:/Projects/c_labs/high_level/output.txt", "w");
  if (out == NULL)
  {
    perror("nonexistent input file");
  }
  while ((c = fgetc(in)) != EOF) {
    if (is_delimiter(c))
      fputc(replace_c, out);
    else 
      fputc(c, out);
  }  
}