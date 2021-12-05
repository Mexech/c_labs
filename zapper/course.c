#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 8192
// #define INIT_DICT_SIZE 1024
#define INIT_DICT_SIZE 270
#define len(a)                      \
    ({int l;                        \
    for (l = 0; l < BUFF_SIZE; l++) \
        if (a[l] == 0)              \
            break;                  \
    l;})

int dict_extended_len = 0;
int dict_size = INIT_DICT_SIZE;

char **dict;

int in_dict(char *seq, char next_char);
int dict_index(char *seq);
void fill_dict(char *dict);
int *lzw_encrypt(char *buff);
char *lzw_decrypt(int *buff);
void lzw_encrypt_file(char *path, char *to_archive);
void lzw_decrypt_file(char *path);
void append_dict(char *seq_address);

void main()
{
    lzw_encrypt_file("in.txt", "out.zap");
    // memset(message, 0, len(message)*sizeof(message[0]));
    // fread(message, 4, BUFF_SIZE, in);
    // memset(dict, 0, sizeof dict);
    // memset(buff, 0, BUFF_SIZE);
    // int i = 0;
    // dict_extended_len = 0;
    // for (i = 1; i < 256; i++)
    // {
    //     seq[0] = i;
    //     char *seq_address = malloc(sizeof strlen(seq));
    //     strcpy(seq_address, seq);
    //     dict[i] = seq_address;
    // }
    // i = 0;
    // memset(seq, 0, sizeof seq);

    // char *result = lzw_decrypt(message);
    // printf(result);

    // if (m)
    //     perror("copy");
}

void init_dict()
{
    char seq[BUFF_SIZE] = {0};
    for (int i = 1; i < 256; i++)
    {
        seq[0] = i;
        char *seq_address = malloc(sizeof strlen(seq));
        strcpy(seq_address, seq);
        dict[i] = seq_address;
    }
}

void lzw_encrypt_file(char *path, char *to_archive)
{
    char buff[BUFF_SIZE] = {0};
    char seq[BUFF_SIZE] = {0};
    dict = calloc(dict_size, sizeof(char *));
    FILE *in = fopen(path, "rb");
    if (in == NULL)
        perror("nonexistant input file");
    FILE *out = fopen(to_archive, "wb+"); // TODO: ab+ if length of paths array nonzero
    if (out == NULL)
        perror("nonexistant output file");
    init_dict();
    do
    {
        size_t n = fread(buff, 1, sizeof buff, in);
        memset(seq, 0, sizeof seq);
        if (n)
        {
            int *message = lzw_encrypt(buff);
            free(dict);
            dict_size = INIT_DICT_SIZE;
            dict_extended_len = 0;
            dict = calloc(dict_size, sizeof(char *));
            init_dict();
            char *result = lzw_decrypt(message); //FIXME: breaks because of these string checks
            printf(result);
            fwrite(message, sizeof(message[0]), len(message), out);
        }
        else
            break;
    } while (1);
    dict_size = INIT_DICT_SIZE;
}

void append_dict(char *seq_address)
{
    if (256 + dict_extended_len + 1 >= dict_size)
    {
        dict_size *= 2;
        dict = realloc(dict, dict_size * sizeof(char *));
    }
    dict[256 + dict_extended_len++] = seq_address;
    dict[256 + dict_extended_len] = NULL;
}

void lzw_decrypt_file(char *path)
{

}

int *lzw_encrypt(char *buff) {
    static int result[BUFF_SIZE] = {0};
    char seq[BUFF_SIZE] = {0};
    int i = 0, i_m = 0, j = 0;
    while ((seq[0] = buff[i++]) != 0)
    {
        j = 0;
        while (1)
        {
            if (!in_dict(seq, buff[i]))
            {
                seq[strlen(seq) - 1] = 0;
                result[i_m++] = dict_index(seq);
                printf("%d ", dict_index(seq));
                seq[++j] = buff[i];
                char *seq_address = malloc(sizeof strlen(seq));
                strcpy(seq_address, seq);
                append_dict(seq_address);
                break;
            }
            else if (buff[i] == 0)
            {
                printf("%d ", dict_index(seq));
                result[i_m++] = dict_index(seq);
                i++;
                break;
            }
            else
                seq[++j] = buff[i++];
        }
        memset(seq, 0, sizeof seq);
    }
    return result;
}

char *lzw_decrypt(int *buff) {
    static char result[BUFF_SIZE] = {0}; // TODO: might need realloc
    char seq[BUFF_SIZE] = {0};
    int i = 0, j = 0;
    strcpy(result, dict[buff[i]]);
    while ((strlen(strcpy(seq, (dict[buff[i + 1]]) ? dict[buff[i++]] : "")) != 0) || buff[++i])
    {
        j = 0;
        while (1) {
            if (!dict[buff[i]])
            {
                if (!strlen(seq))
                    strcpy(seq, dict[buff[i - 1]]);
                seq[strlen(seq)] = seq[0];
                strcat(result, seq);
                char *seq_address = malloc(sizeof strlen(seq));
                strcpy(seq_address, seq);
                append_dict(seq_address);
                break;
            }
            else if (!in_dict(seq, dict[buff[i]][j]))
            {
                strcat(result, dict[buff[i]]);
                char *seq_address = malloc(sizeof strlen(seq));
                strcpy(seq_address, seq);
                append_dict(seq_address);
                break;
            }
            else
                seq[++j] = dict[buff[i + 1]][j];
        }
        memset(seq, 0, sizeof seq);
    }
    return result;
}

int in_dict(char *seq, char next_char)
{
    int k;
    seq[strlen(seq)] = next_char;
    for (k = 256 + dict_extended_len; k >= 0; k--)
    {
        if (!strcmp((dict[k] == 0) ? "" : dict[k], seq))
        {
            return 1;
        }
    }
    return 0;
}

int dict_index(char *seq)
{
    int k;
    for (k = 256 + dict_extended_len; k >= 0; k--)
        if (!strcmp((dict[k] == 0) ? "" : dict[k], seq))
            return k;
}

void fill_dict(char *dict) {
    memset(dict, 0, sizeof dict);
}