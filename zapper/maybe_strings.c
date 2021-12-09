#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 8192
#define RESULT_SIZE 8192
#define INIT_DICT_SIZE 1024
#define len(a)                      \
    ({int l;                        \
    for (l = 0; l < BUFF_SIZE; l++) \
        if (a[l] == 0)              \
            break;                  \
    l;})

int dict_extended_len = 0;
int dict_size = INIT_DICT_SIZE;

int *lzw_encrypt(char *buff, FILE *from_file, int amount);
char *lzw_decrypt(int *buff, FILE *from_arch);
void lzw_encrypt_file(char *path, FILE *to_arch);
void lzw_decrypt_file(char *path, FILE *from_arch);
// void append_dict(char *seq_address);
int in_dict(char *seq, int len);
int streq(char *s1, char *s2, int len);
int dict_index(char *seq);
void fill_dict(char *dict);
void init_dict();
// void reset_dict();
int fpeek(FILE *const fp);

typedef struct _string
{
    char *buffer;
    size_t len;
    size_t capacity;
    struct _string *next
} string;
struct _dict
{
    string *start;
    string *end;
};

struct _dict *dict;

string *new_string(int num)
{
    string *str = malloc(sizeof(string));
    str->buffer = malloc(num);
    str->len = 0;
    str->capacity = num;
    str->next = dict->end;
    dict->end = str;
    return str;
}

string *append(string *str, char c)
{
    if (str->capacity == str->len)
    {
        str->capacity *= 1.5;
        str->buffer = realloc(str->buffer, str->capacity);
    }
    str->buffer[str->len] = c;
    str->len += 1;
    return str;
}

string *trim(string *str)
{
    str->buffer = realloc(str->buffer, str->len);
    str->capacity = str->len;
}

void main()
{
    init_dict();
    // FILE *out = fopen("out.zap", "wb+");
    // if (out == NULL)
    //     perror("nonexistant output file");
    // lzw_encrypt_file("in.zap", out);
    // fclose(out);
    // FILE *in = fopen("out.zap", "rb+");
    // if (in == NULL)
    //     perror("nonexistant output file");
    // lzw_decrypt_file("decrypted.zap", in);
}

// void reset_dict()
// {
//     free(dict);
//     dict_size = INIT_DICT_SIZE;
//     dict_extended_len = 0;
//     init_dict();
// }

void init_dict()
{
    dict = malloc(sizeof(struct _dict));
    dict->end = new_string(1);
    append(dict->end, 1);
    dict->start = new_string(1);
    append(dict->start, 0);
    for (int i = 1; i <= 256; i++)
    {
        string *seq = new_string(1);
        append(seq, i);
    }
    string *s = dict->start;
    while (s->next != dict->end)
    {
        printf(s->buffer);
        s = s->next;
    }
    // dict = calloc(dict_size, sizeof(char *));
    // for (char i = 1; i < 256; i++)
    // {
    //     string *seq = new_string(1);
    //     append(seq, i);
    //     dict[i] = seq;
    // }
}

// void lzw_encrypt_file(char *path, FILE *to_arch)
// {
//     char buff[BUFF_SIZE] = {0};
//     char seq[BUFF_SIZE] = {0};
//     FILE *in = fopen(path, "rb");
//     if (in == NULL)
//         perror("nonexistant input file");
//     do
//     {
//         size_t n = fread(buff, sizeof(buff[0]), sizeof(buff), in);
//         memset(seq, 0, sizeof(seq));
//         if (n)
//         {
//             if (n < sizeof(buff))
//                 buff[n] = 0;
//             int *message = lzw_encrypt(buff, in, n);
//             int l; for (l = 0; message[l] != 0; l++);
//             fwrite(message, sizeof(message[0]), l, to_arch);
//             free(message);
//         }
//         else
//             break;
//     } while (1);
//     reset_dict();
// }

// void append_dict(char *seq_address)
// {
//     if (256 + dict_extended_len + 1 >= dict_size)
//     {
//         dict_size *= 1.5;
//         dict = realloc(dict, dict_size * sizeof(char *));
//     }
//     dict[256 + dict_extended_len++] = seq_address;
//     dict[256 + dict_extended_len] = NULL;
// }

// void lzw_decrypt_file(char *path, FILE *from_arch)
// {
//     int buff[BUFF_SIZE] = {0};
//     char seq[BUFF_SIZE] = {0};
//     int size = 0;
//     FILE *out = fopen(path, "wb+");
//     if (out == NULL)
//         perror("nonexistant input file");
//     do
//     {
//         size_t n = fread(buff, sizeof(buff[0]), BUFF_SIZE - 1, from_arch);
//         size += n;
//         memset(seq, 0, sizeof(seq));
//         if (n)
//         {
//             if (n < BUFF_SIZE - 1)
//                 buff[n] = 0;
//             char *message = lzw_decrypt(buff, from_arch);
//             int l; for (l = 0; message[l] != 0; l++);
//             fwrite(message, sizeof(message[0]), l, out);
//             free(message);
//         }
//         else
//             break;
//     } while (1);
//     reset_dict();
// }

// int *lzw_encrypt(char *buff, FILE *from_file, int amount) {
//     int *result = calloc(BUFF_SIZE, sizeof(int));
//     char seq[BUFF_SIZE] = {0};
//     int i = 0, i_m = 0, seq_len = 0;
//     while (i < amount && i < BUFF_SIZE)
//     {
//         seq_len = 0;
//         seq[seq_len++] = buff[i++];
//         while (1)
//         {
//             seq[seq_len++] = i >= amount ? fpeek(from_file) : buff[i];
//             if (!in_dict(seq, seq_len))
//             {
//                 seq[--seq_len] = 0;
//                 result[i_m++] = dict_index(seq); // FIXME: will throw if couldn't compress buff!!!
//                 int t = dict_index(seq);
//                 printf("%d ", dict_index(seq));
//                 if (i >= BUFF_SIZE)
//                     seq[++seq_len] = fpeek(from_file);
//                 else 
//                     seq[++seq_len] = buff[i];
//                 char *seq_address = malloc(strlen(seq));
//                 strcpy(seq_address, seq);
//                 append_dict(seq_address);
//                 break;
//             }
//             else if (i >= BUFF_SIZE) // found the end of buffer not the end of message
//             {
//                 seq[++seq_len] = fgetc(from_file);
//                 i++;
//             }
//             else
//                 seq[++seq_len] = buff[i++];
//         }
//         memset(seq, 0, sizeof seq);
//     }
//     return result;
// }

// char *lzw_decrypt(int *buff, FILE *from_arch) {
//     int result_size = RESULT_SIZE;
//     char *result = calloc(result_size, sizeof(char));
//     char seq[BUFF_SIZE] = {0};
//     int i = 0, j = 0;
//     if (dict[256] == NULL) // checking wether decrypting first batch
//         strcpy(result, dict[buff[i]]);
//     while ((strlen(strcpy(seq, (dict[buff[i + 1]]) ? dict[buff[i++]] : "")) != 0) || buff[++i])
//     {
//         j = 0;
//         while (1) {
//             if (!dict[buff[i]])
//             {
//                 if (!strlen(seq))
//                     strcpy(seq, dict[buff[i - 1]]);
//                 seq[strlen(seq)] = seq[0];
//                 if (strlen(result) + strlen(seq) + 1 >= result_size)
//                 {
//                     result_size *= 1.5;
//                     result = realloc(result, result_size);
//                 }
//                 strcat(result, seq);
//                 char *seq_address = malloc(strlen(seq));
//                 strcpy(seq_address, seq);
//                 append_dict(seq_address);
//                 break;
//             }
//             else if (!in_dict(seq, dict[buff[i]][j]))
//             {
//                 if (strlen(result) + strlen(dict[buff[i]]) + 1 >= result_size)
//                 {
//                     result_size *= 1.5;
//                     result = realloc(result, result_size);
//                 }
//                 strcat(result, dict[buff[i]]);
//                 char *seq_address = malloc(strlen(seq));
//                 strcpy(seq_address, seq);
//                 append_dict(seq_address);
//                 break;
//             }
//             else if (dict[buff[i + 1]] != NULL)
//                 seq[++j] = dict[buff[i + 1]][j];
//         }
//         memset(seq, 0, sizeof seq);
//     }
//     if (i == BUFF_SIZE - 1)
//         fseek(from_arch, -sizeof(buff[0]), SEEK_CUR);
//     return result;
// }

// int streq(char *s1, char *s2, int len)
// {
//     for (int i = 0; i < len; i++)
//         if (s1[i] != s2[i])
//             return 0;
//     return 1;
// }

// int in_dict(char *seq, int len)
// {
//     for (int k = 255 + dict_extended_len; k >= 0; k--)
//         if (streq(dict[k], seq, len))
//             return 1;
//     return 0;
// }

// int dict_index(char *seq)
// {
//     int k;
//     for (k = 256 + dict_extended_len; k >= 0; k--)
//         if (!strcmp((dict[k] == 0) ? "" : dict[k], seq))
//             return k;
// }

// void fill_dict(char *dict) {
//     memset(dict, 0, sizeof dict);
// }

// int fpeek(FILE *const fp)
// {
//     const int c = getc(fp);
//     return c == EOF ? EOF : ungetc(c, fp);
// }