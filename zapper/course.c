#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 8192
#define DICT_SIZE 1024000

char *dict[DICT_SIZE];

int dict_extended_len = 0;

int in_dict(char *seq, char next_char);
int dict_index(char *seq);
void fill_dict(char *dict);
int *lzw_encrypt(char *buff, char *dict);

void main()
{
    FILE *in, *out;

    in = fopen("F:/Projects/c_labs/zapper/owo.txt", "rb");
    if (in == NULL) {
        perror("nonexistant input file");
    }
    out = fopen("F:/Projects/c_labs/zapper/wow.txt", "wb");
    if (out == NULL) {
        perror("nonexistant output file");
    }
    size_t n, m;
    char buff[BUFF_SIZE] = {0};
    int *message;
    char seq[BUFF_SIZE] = {0};
    for (int i = 1; i < 256; i++)
    {
        seq[0] = i;
        char *seq_address = malloc(sizeof strlen(seq));
        strcpy(seq_address, seq);
        dict[i] = seq_address;
    }
    do {
        n = fread(buff, 1, sizeof buff, in);
        int i_m = 0;
        memset(seq, 0, sizeof seq);
        if (n) {
            message = lzw_encrypt(buff, dict);
        }
        else
        {
            m = 0;
        }
    } while ((n > 0) && (m == 1));

    memset(dict, 0, sizeof dict);
    int i = 0;
    char seq[BUFF_SIZE] = {0};
    dict_extended_len = 0;
    for (i = 1; i < 256; i++)
    {
        seq[0] = i;
        char *seq_address = malloc(sizeof strlen(seq));
        strcpy(seq_address, seq);
        dict[i] = seq_address;
    }
    i = 0;
    memset(seq, 0, sizeof seq);

    printf(dict[message[i]]);
    while ((strlen(strcpy(seq, (dict[message[i + 1]]) ? dict[message[i++]] : "")) != 0) || message[++i])
    {
        int j = 0;
        while (1) {
            if (!dict[message[i]])
            {
                if (!strlen(seq))
                    strcpy(seq, dict[message[i - 1]]);
                seq[strlen(seq)] = seq[0];
                printf(seq);
                char *seq_address = malloc(sizeof strlen(seq));
                strcpy(seq_address, seq);
                dict[256 + dict_extended_len++] = seq_address;
                break;
            }
            else if (!in_dict(seq, dict[message[i]][j]))
            {
                printf(dict[message[i]]);
                char *seq_address = malloc(sizeof strlen(seq));
                strcpy(seq_address, seq);
                dict[256 + dict_extended_len++] = seq_address;
                break;
            }
            else
                seq[++j] = dict[message[i + 1]][j];
        }
        memset(seq, 0, sizeof seq);
    } 

    // if (m)
    //     perror("copy");
    if (fclose(out))
        perror("close output file");
    if (fclose(in))
        perror("close input file");
}

int *lzw_encrypt(char *buff, char *dict) {
    int result[BUFF_SIZE] = {0};
    char seq[BUFF_SIZE] = {0};
    int i = 0, i_m = 0;
    while ((seq[0] = buff[i++]) != 0)
    {
        int j = 0;
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
                dict[256 + dict_extended_len++] = seq_address;
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

int in_dict(char *seq, char next_char)
{
    int k;
    seq[strlen(seq)] = next_char;
    for (k = 256 + dict_extended_len; k >= 0; k--)
    {
        if (!strcmp((dict[k] == 0) ? "" : dict[k], seq)){
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