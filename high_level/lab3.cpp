#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN 1024
//По запросу программа должна выравнивать текст,
//записанный в файле, состоящий из строк, по 
//а) центру, б) левому краю, в) правому краю.
int is_space(int c)
{
  return c == ' ' || c == '\t';
}

int fpeek(FILE *const fp)
{
  const int c = getc(fp);
  return c == EOF ? EOF : ungetc(c, fp);
}

int main()
{
  char c, prev_c = 0;
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
  int max_new_line = 0;
  int i = 0;
  while ((c = fgetc(in)) != EOF)
  {
    i++;
    if (c == '\n')
    {
      if (i > max_new_line)
        max_new_line = i;
      i = 0;
    }    
  }
  rewind(in);
  char action;
  scanf("%c", &action);
  int found_c = 0;
  char compressed_line[MAX_LINE_LEN] = {0};
  char spaces[MAX_LINE_LEN] = {0};
  i = 0;
  c = fgetc(in);
  do
  {
    if (!is_space(c) && !found_c)
    {
      found_c = 1;
    }
    if (found_c && (!is_space(prev_c) && is_space(c) || !is_space(c)))
    {
      compressed_line[i++] = c;
    }
    if (c == '\n' || fpeek(in) == EOF) {
      found_c = 0;
      i = 0;
      if (action == '|') {
        memset(spaces, ' ', (max_new_line - strlen(compressed_line)) / 2);
      }
      else if (action == '>')
      {
        memset(spaces, ' ', max_new_line - strlen(compressed_line) - 1 * (fpeek(in) == EOF));
      }
      fprintf(out, strcat(spaces, compressed_line));
      memset(compressed_line, 0, strlen(compressed_line));
      memset(spaces, 0, strlen(spaces));
    }
    prev_c = c;
  } while ((c = fgetc(in)) != EOF);
  return 0;
}