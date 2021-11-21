#include <stdio.h>
#include <string.h>

#define INIT_STRING_SIZE 1000

static const char *types[] = {
    "char",
    "signed",
    "unsigned",
    "short",
    "int",
    "long",
    "float",
    "double",
    "void",
};

int is_delimiter(int c)
{
    return c == ' ' || c == EOF || c == '\n' || c == '\r' || c == 0;
}

void concat_to_rest(char *word, char *rest, int after_par)
{
    if (!strcmp(word, ",") && rest[strlen(rest) - 1] == ' ')
        rest[strlen(rest) - 1] = 0;
    if (after_par)
        strcat(word, " ");
    strcat(rest, word);
}

void main() {
    int i, len, j, file_end, in_word, word_len, get_rest, found_equals, has_curly, legit, after_par;
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
        char *buff = calloc(INIT_STRING_SIZE, sizeof(char));
        char type[INIT_STRING_SIZE] = {0};
        char word[INIT_STRING_SIZE] = {0};
        char rest[INIT_STRING_SIZE] = {0};
        in_word = 0; word_len = 0;
        found_equals = 0; has_curly = 0; legit = 0;
        after_par = 0; get_rest = 0; j = 0;
        *(buff + j) = fgetc(in);
        do
        {
            if (is_delimiter(*(buff + j - 1)) && !is_delimiter(*(buff + j)))
                in_word = 1;
            else if (is_delimiter(*(buff + j)) || *(buff + j) == ';')
            {
                in_word = 0;
                word_len = 0;
                if (get_rest && strlen(word))
                    concat_to_rest(word, rest, after_par);
                else if (strlen(word)) {
                    int t, extended_type;
                    extended_type = 0;
                    for (t = 0; t < 9; t++)
                        if (!strcmp(word, types[t]))
                        {
                            extended_type = 1;
                            strcat(type, strcat(word, " "));
                            break;
                        }
                    if (strlen(type) && !extended_type)
                    {
                        concat_to_rest(word, rest, after_par);
                        get_rest = 1;
                    }
                }
                memset(word, 0, sizeof word);
            }

            if (in_word) 
                word[word_len++] = buff[j];

            if (*(buff + j) == EOF)
            {
                *(buff + j) = 0;
                file_end = 1;
                break;
            }
            else if (*(buff + j) == '=')
                found_equals = 1;
            else if (*(buff + j) == '{')
                if (strlen(type))
                    has_curly = 1;
                else
                    break;
            else if (*(buff + j - 1) == '(')
                after_par = 1;
            if (!found_equals && has_curly && strlen(type))
            {
                legit = 1;
                if (in_word)
                    strcat(rest, word);
                break;
            }
            j++;
            if (!(j % INIT_STRING_SIZE)) {
                int extended_len = ((j / INIT_STRING_SIZE) + 1) * INIT_STRING_SIZE * sizeof(char) + 1;
                buff = realloc(buff, extended_len);
                buff[extended_len - 1] = 0;
            }
        } while ((*(buff + j) = fgetc(in)) != ';');
        if (legit) {
            int t = 3;
            if (rest[strlen(rest) - 4] == ' ')
                t = 4;
            strcpy(rest + strlen(rest) - t, ");\n\0");
            fputs(strcat(type, rest), out);
            // printf(strcat(type, rest));
        }
    }
}