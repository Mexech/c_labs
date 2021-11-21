#include <stdio.h>
#include <string.h>

#define MAX_STRING_SIZE 1000

int is_delimiter(int c)
{
    return c == ' ' || c == '.' || c == ',' || c == '\r' || c == '\n';
}

void main() {
    int i, len, i_prev, j, file_end;
    FILE *in, *out;

    in = fopen("F:/Projects/c_labs/input.txt", "rb");
    if (in == NULL)
    {
        perror("nonexistent input file");
    }
    out = fopen("F:/Projects/c_labs/output.txt", "wb");
    if (out == NULL)
    {
        perror("nonexistent output file");
    }
    file_end = 0;
    while (!file_end) {
        char result[MAX_STRING_SIZE] = {0};
        i_prev = -1;
        j = 0;
        while ((*(result + j) = fgetc(in)) != '\n' && j < MAX_STRING_SIZE)
        {
            if (*(result + j) == EOF)
            {
                *(result + j) = 0;
                file_end = 1;
                break;
            }
            if (is_delimiter(*(result + j - 1)) && !is_delimiter(*(result + j)))
            {
                i_prev = i;
                i = j - 1;
            }
            j++;
        }
        if (i_prev == 0 && is_delimiter(*(result + i_prev)))
            i_prev = -1;
        len = j;
        if (i_prev != -1)
            for (j = i_prev + 1; j < len; j++)
            {
                *(result + j) = *(result + j + i - i_prev);
                *(result + j + i - i_prev) = 0;
            }

        fputs(result, out);
    }
}