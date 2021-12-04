#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 8192
#define DICT_SIZE 1024

#define len(a)                      \
    ({int l;                        \
    for (l = 0; l < BUFF_SIZE; l++) \
        if (a[l] == 0)              \
            break;                  \
    l;})

char *dict[DICT_SIZE];
int dict_extended_len = 0;

int in_dict(char *seq, char next_char);
int dict_index(char *seq);
void fill_dict(char *dict);
int *lzw_encrypt(char *buff);
char *lzw_decrypt(int *buff);

void main()
{
    FILE *in, *out;
    in = fopen("in.txt", "rb");
    if (in == NULL) {
        perror("nonexistant input file");
    }
    out = fopen("out.zap", "wb+");
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
            message = lzw_encrypt(buff);
            fwrite(message, sizeof(message[0]), len(message), out);
        }
        else
        {
            m = 0;
        }
    } while ((n > 0) && (m == 1));
    fclose(out);
    in = fopen("out.zap", "rb");
    if (in == NULL)
    {
        perror("nonexistant input file");
    }
    memset(message, 0, len(message)*sizeof(message[0]));
    fread(message, 4, BUFF_SIZE, in);
    memset(dict, 0, sizeof dict);
    memset(buff, 0, BUFF_SIZE);
    int i = 0;
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

    char *result = lzw_decrypt(message);
    printf(result);

    // if (m)
    //     perror("copy");
    if (fclose(out))
        perror("close output file");
    if (fclose(in))
        perror("close input file");
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

char *lzw_decrypt(int *buff) {
    static char result[BUFF_SIZE] = {0}; // TODO: might need realloc
    char seq[BUFF_SIZE] = {0};
    int i = 0, i_m = 0, j = 0;
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
                dict[256 + dict_extended_len++] = seq_address;
                break;
            }
            else if (!in_dict(seq, dict[buff[i]][j]))
            {
                strcat(result, dict[buff[i]]);
                char *seq_address = malloc(sizeof strlen(seq));
                strcpy(seq_address, seq);
                dict[256 + dict_extended_len++] = seq_address;
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