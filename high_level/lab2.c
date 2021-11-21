#include <stdio.h>

void main()
{
  int c;
  FILE *in, *out;
  in = fopen("F:/Projects/c_labs/high_level/input.txt", "r");
  if (in == NULL)
  {
      perror("nonexistent output file");
  }
  out = fopen("F:/Projects/c_labs/high_level/output.txt", "w");
  if (out == NULL)
  {
      perror("nonexistent output file");
  }
  while ((c = fgetc(in)) != EOF) 
    if (c > 47 && c < 58) 
      fputc('!', out);
    else
      fputc(c, out);
}