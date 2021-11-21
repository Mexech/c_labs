#include <stdio.h>
#include <string.h>
#include <windows.h>

#define MAX_STRING_SIZE 1000
#define DICT_SIZE 1000

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

char dict[DICT_SIZE][MAX_STRING_SIZE];
int dict_len = 0;

int is_delimiter(int c)
{
    return c == ' ' || c == ';' || c == '=' || c == '!' || c == '+' || c == '-' || c == ',' || c == '(' || c == '*' || c == ')' || c == '[' || c == '{' || c == '}' || c == ']' || c == '\n' || c == '\r' || c == 0;
}

int in_dict(char *word) {
    int i;
    for (i = dict_len; i >= 0; i--)
        if (!strcmp(dict[i], word))
            return 1;
    return 0;
}

void main()
{
    int i, len, j, file_end, in_word, word_len, last_char_i;
    FILE *in;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);

    in = fopen("F:/Projects/c_labs/input.txt", "rb");
    if (in == NULL)
    {
        perror("nonexistant input file");
    }
    
    file_end = 0;
    while (!file_end) {
        char buff[MAX_STRING_SIZE] = {0};
        char type[MAX_STRING_SIZE] = {0};
        char word[MAX_STRING_SIZE] = {0};
        char rest[MAX_STRING_SIZE] = {0};

        in_word = 0; word_len = 0;
        j = 0; last_char_i = 0;
        *(buff + j) = fgetc(in);
        do
        {
            if (*(buff + j) == '=')
                memset(type, 0, sizeof type);
            if (is_delimiter(*(buff + j - 1)) && !is_delimiter(*(buff + j)))
                in_word = 1;
            else if (is_delimiter(*(buff + j)) || *(buff + j) == EOF)
            {
                in_word = 0;
                word_len = 0;
                if (strlen(word)) {
                    int t, extended_type;
                    extended_type = 0;
                    for (t = 0; t < 9; t++)
                        if (!strcmp(word, types[t]))
                        {
                            extended_type = 1;
                            strcat(type, strcat(word, " "));
                            break;
                        }
                    int found = in_dict(word);
                    if ((strlen(type) && !extended_type) || found)
                    {
                        printf("%.*s", strlen(buff) - last_char_i - strlen(word) - 1, buff + last_char_i);
                        SetConsoleTextAttribute(hConsole, 3);
                        printf(word);
                        SetConsoleTextAttribute(hConsole, 7);
                        last_char_i = j;
                        !found ? strcpy(dict[dict_len++], word) : 0;
                    } 
                }
                memset(word, 0, sizeof word);
            }

            if (in_word)
                word[word_len++] = *(buff + j);

            if (*(buff + j) == EOF)
            {
                *(buff + j) = 0;
                file_end = 1;
                break;
            }
            else if (*(buff + j) == '{')
                break;
            j++;
        } while ((*(buff + j) = fgetc(in)) != ';' && j < MAX_STRING_SIZE);
        if (is_delimiter(*(buff + j)) || *(buff + j) == EOF)
            if (strlen(word))
            {
                int found = in_dict(word);
                if (strlen(type) || found)
                {
                    printf("%.*s", strlen(buff) - last_char_i - strlen(word) - 1, buff + last_char_i);
                    SetConsoleTextAttribute(hConsole, 3);
                    printf(word);
                    SetConsoleTextAttribute(hConsole, 7);
                    last_char_i = j;
                    if (!found)
                        strcpy(dict[dict_len++], word);
                }
            }
        printf("%.*s", strlen(buff) - last_char_i, buff + last_char_i);
    }
}