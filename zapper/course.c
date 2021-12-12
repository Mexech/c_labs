#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define PATHS_AMOUNT 1024
#define PATH_SIZE 1024
#define BUFF_SIZE 8192
#define RESULT_SIZE 8192
#define INIT_DICT_SIZE 1024

int dict_extended_len = 0;
int seq_len = 0;
int paths_len = 0;
int result_len = 0;
int i_m = 0;
int eocf = 0;
int dict_size = INIT_DICT_SIZE;

int **dict;
char **paths;

int in_dict(int *seq);
int dict_index(int *seq);
void fill_dict(char *dict);
int *lzw_encrypt(unsigned char *buff, FILE *from_file, int amount);
void lzw_encrypt_file(char *path, FILE *to_arch);
char *lzw_decrypt(int *buff, FILE *from_arch, int amount);
void lzw_decrypt_file(char *path, FILE *from_arch);
void append_dict(int *seq_address);
void reset_dict();
void init_dict();
int fpeek(FILE *const fp);
int streq(int *s1, int *s2);
void append(int *seq, int c);
void concat_r(char *res, int *seq);
void concat_s(int *seq1, int *seq2);
int len(int *seq);

char *origin_folder(char *path)
{
    char *origin = calloc(PATH_SIZE, 1);
    int i = strlen(path), j = 0;
    while (((origin[j++] = path[--i]) != '/') && (origin[j - 1] != '\\'));
    i = 0;
    while (i < j / 2)
    {
        char t = origin[j - i - 1];
        origin[j - i - 1] = origin[i];
        origin[i++] = t;
    }
    return origin;
}

char *strremove(char *str, const char *sub)
{
    char *p, *q, *r;
    if (*sub && (q = r = strstr(str, sub)) != NULL)
    {
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL)
        {
            while (p < r)
                *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0')
            continue;
    }
    return str;
}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void paths2file(char *origin)
{
    FILE *paths_file = fopen("paths.tmp", "wb+");
    for (int i = 0; i < paths_len; i++)
    {
        char *path = calloc(PATH_SIZE, 1);
        strcpy(path, paths[i]);
        fprintf(paths_file, strcat(strcat(origin_folder(origin), strremove(path, origin)), "\n"));
        free(path);
    }
    fclose(paths_file);
}

void fill_paths(const char *name)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        char *path = calloc(PATH_SIZE, sizeof(char));
        strcpy(path, name);
        snprintf(path, PATH_SIZE, "%s/%s", name, entry->d_name);
        if (is_regular_file(path))
        {
            // printf(path);
            // printf("\n");
            paths[paths_len++] = path;
        }
        else
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            // printf(path);
            // printf("\n");
            fill_paths(path);
        }
    }
    closedir(dir);
}

void process_paths(char *prefix)
{
    FILE *paths_file = fopen("paths.tmp", "rb");
    char c;
    int i = strlen(prefix);
    char *path = calloc(PATH_SIZE, sizeof(char));
    strcpy(path, prefix);
    while ((c = fgetc(paths_file)) != EOF)
    {
        if (c == '\n')
        {
            i = strlen(prefix);
            paths[paths_len++] = path;
            path = calloc(PATH_SIZE, sizeof(char));
            strcpy(path, prefix);
        }
        else
            path[i++] = c;
    }
    fclose(paths_file);
    remove("paths.tmp");
}

char *without_filename(char *path)
{
    char *result = calloc(PATH_SIZE, 1);
    strcpy(result, path);
    int j = strlen(result) - 1;
    while (result[j--] = 0, (result[j] != '/') && (result[j] != '\\'));
    for (int i = 0; i < strlen(result); i++)
        if (result[i] == '/')
            result[i] = '\\';
    return result;
}

int main()
{
    paths = malloc(PATHS_AMOUNT * sizeof(char *));
    char *path_to_encrypt = "C:/Users/maksp/Desktop/test";
    char *path_to_decrypt = calloc(PATH_SIZE, 1);
    strcpy(path_to_decrypt, "C:/Users/maksp/Desktop/Decrypted");
    if (is_regular_file(path_to_encrypt))
        paths[paths_len++] = path_to_encrypt;
    else
        fill_paths(path_to_encrypt);
    init_dict();

    FILE *out = fopen("out.zap", "wb+");
    paths2file(path_to_encrypt);
    lzw_encrypt_file("paths.tmp", out);
    remove("paths.tmp");
    for (int i = 0; i < paths_len; i++)
    {
        lzw_encrypt_file(paths[i], out);
        free(paths[i]);
    }
    free(paths);
    paths = malloc(PATHS_AMOUNT * sizeof(char *));
    paths_len = 0;
    fclose(out);
    
    FILE *in = fopen("out.zap", "rb+");
    if (in == NULL)
        perror("nonexistant output file");
    lzw_decrypt_file("paths.tmp", in);
    process_paths(path_to_decrypt);
    for (int i = 0; i < paths_len; i++)
    {
        lzw_decrypt_file(paths[i], in);
        free(paths[i]);
    }
    free(paths);
    paths_len = 0;
    return 0;
}

char *pack(int *message)
{
    char *buff = calloc(i_m*4, sizeof(char));
    int j = 0, shift = 0;
    message[0] = 40034;
    while ((buff[j++] = (message[0] >> (8 * j)) & 0xff) != 0);
    while (((buff[j-2] << ++shift) & 0x80) != 128);
    buff[j-2] = buff[j-2] << shift;
    for (int i = j - 2; i < i_m; i++)
    {
        j = 0;
        while (buff[i + j] != 0)
        {
            buff[i + j] = buff[i + j] | ((message[i] >> (8 * j)) & ((1 << shift) - 1));
            message[i] = message[i] >> shift;
            shift = 0;
            j++;
        }
        // while (((buff[j - 2] << ++shift) & 0x80) != 128);
        buff[j - 2] = buff[j - 2] << shift;
    }
    return buff;
}

void lzw_encrypt_file(char *path, FILE *to_arch)
{
    unsigned char buff[BUFF_SIZE] = {0};
    FILE *in = fopen(path, "rb");
    fseek(in, 0L, SEEK_END);
    int sz = ftell(in);
    fseek(in, 0, SEEK_SET);
    if (sz == 0)
    {
        int message[] = {256};
        fwrite(message, sizeof(int), 1, to_arch);
    }
    if (in == NULL)
        perror("nonexistant input file");
    do
    {
        size_t n = fread(buff, sizeof(buff[0]), sizeof(buff), in);
        printf("%d\n", sz);
        sz -= n;
        if (n)
        {
            int *message = lzw_encrypt(buff, in, n);
            if (n < BUFF_SIZE)
                message[i_m++] = 256 + dict_extended_len;
            fwrite(message, sizeof(message[0]), i_m, to_arch);
            free(message);
        }
        else            
            break;

    } while (1);
    fclose(in);
    reset_dict();
}

int *lzw_encrypt(unsigned char *buff, FILE *from_file, int amount)
{
    int *result = calloc(BUFF_SIZE, sizeof(int));
    int i = 0, last = 0;
    i_m = 0;
    while (i < amount && i < BUFF_SIZE)
    {
        int *seq = malloc(sizeof(int) * BUFF_SIZE);
        seq_len = 0;
        append(seq, buff[i++]);
        while (1)
        {
            append(seq, i < amount ? buff[i] : fgetc(from_file));
            if (!in_dict(seq))
            {
                last = seq[--seq_len];
                seq[seq_len] = 256;
                result[i_m++] = dict_index(seq);
                append(seq, last);
                append_dict(seq);
                break;
            } 
            else
                i++;
        }
    }
    if (i >= amount)
        ungetc(last, from_file);
    return result;
}

void lzw_decrypt_file(char *path, FILE *from_arch)
{
    int buff[BUFF_SIZE] = {0};
    if (paths_len) 
    {
        char *command = calloc(PATH_SIZE, 1);
        strcpy(command, "mkdir \"");
        strcat(command, without_filename(path));
        strcat(command, "\" >nul 2>nul");
        system(command);
    }
    eocf = 0;
    FILE *out = fopen(path, "wb+");
    // int cur = ftell(from_arch);
    // fseek(from_arch, 0L, SEEK_END);
    // int sz = ftell(from_arch);
    // fseek(from_arch, cur, SEEK_SET);
    if (out == NULL)
        perror("nonexistant input file");
    do
    {
        size_t n = fread(buff, sizeof(buff[0]), BUFF_SIZE - 1, from_arch);
        // printf("%d\n", sz);
        // sz -= (n - 1)*sizeof(int);
        if (n)
        {           
            char *message = lzw_decrypt(buff, from_arch, n);
            fwrite(message, sizeof(message[0]), result_len, out);
            free(message);
        }
        else
            break;
    } while (!eocf);
    fclose(out);
    reset_dict();
}

void concat_r(char *res, int *seq)
{
    int i;
    for (i = 0; seq[i] != 256; i++)
        res[result_len++] = seq[i];      // TODO: realloc here
}

void concat_s(int *seq1, int *seq2)
{
    int l1 = len(seq1);
    for (int i = 0; seq2[i] != 256; i++)
        seq1[l1++] = seq2[i];           // TODO: realloc here
    seq1[l1] = 256;
    seq_len = l1;
}

char *lzw_decrypt(int *buff, FILE *from_arch, int amount) {
    int result_cap = RESULT_SIZE;
    result_len = 0;
    char *result = calloc(result_cap, sizeof(char));
    int i = 0, k = 0;
    if (dict[256] == NULL) // checking wether decrypting first batch
    {
        if (buff[0] == 256) // if file empty
        {
            eocf = 1;
            fseek(from_arch, -(amount - (i + 1)) * sizeof(buff[0]), SEEK_CUR);
        } 
        else
            concat_r(result, dict[buff[i]]);
    } 
    while (i < amount - 1 && i < BUFF_SIZE && !eocf)
    {
        int *seq = malloc(sizeof(int) * BUFF_SIZE);
        seq_len = 0; seq[seq_len] = 256; k = 0;
        concat_s(seq, dict[buff[i++]]);
        while (buff[i] != 256 + dict_extended_len + 1)
        {
            if (!dict[buff[i]])
                append(seq, seq[0]);
            else
                append(seq, dict[buff[i]][k++]);
            if (!in_dict(seq))
            {
                append_dict(seq);
                if (result_len + len(dict[buff[i]]) + 1 >= result_cap)
                {
                    result_cap *= 1.5;
                    result = realloc(result, result_cap);
                }
                concat_r(result, dict[buff[i]]);
                break;
            }
            else
                i++;
        }
        if (buff[i] == 256 + dict_extended_len + 1)
        {
            eocf = 1;
            if (amount > i + 1)
                fseek(from_arch, -(amount - (i + 1))*sizeof(buff[0]), SEEK_CUR);
        }
    }
    if (i == BUFF_SIZE - 2)
        fseek(from_arch, -sizeof(buff[0]), SEEK_CUR);
    return result;
}

void reset_dict()
{
    for (int i = 0; i < 256 + dict_extended_len; i++)
        free(dict[i]);
    free(dict);
    dict_size = INIT_DICT_SIZE;
    dict_extended_len = 0;
    init_dict();
}

void init_dict()
{
    dict = calloc(dict_size, sizeof(int *));
    for (int i = 0; i < 256; i++)
    {
        int *seq = malloc(2 * sizeof(int));
        seq[0] = i;
        seq[1] = 256;
        dict[i] = seq;
    }
}

void append_dict(int *seq_address) // TODO: add seq trimming
{
    if (256 + dict_extended_len + 1 >= dict_size)
    {
        dict_size *= 1.5;
        dict = realloc(dict, dict_size * sizeof(int *));
    }
    dict[256 + dict_extended_len++] = seq_address;
    dict[256 + dict_extended_len] = NULL;
}

int in_dict(int *seq)
{
    int k;
    for (int k = 255 + dict_extended_len; k >= 0; k--)
        if (streq(dict[k], seq))
            return 1;
    return 0;
}

int dict_index(int *seq)
{
    int k;
    for (k = 255 + dict_extended_len; k >= 0; k--)
        if (streq(dict[k], seq))
            return k;
}

void fill_dict(char *dict) {
    memset(dict, 0, sizeof dict);
}

int fpeek(FILE *const fp)
{
    const int c = getc(fp);
    return c == EOF ? EOF : ungetc(c, fp);
}

int streq(int *s1, int *s2)
{
    int i;
    for (i = 0; s1[i] != 256 || s2[i] != 256; i++)
        if (s1[i] != s2[i])
            return 0;
    if ((s1[i] == 256) && (s2[i] == 256))
        return 1;
    else
        return 0;
}

void append(int *seq, int c)
{
    seq[seq_len++] = c;
    seq[seq_len] = 256;
}

void print_dict()
{
    printf("\n");
    for (int i = 256; i < 256 + dict_extended_len; i++)
    {
        int k = 0;
        print(dict[i]);
        printf("\n");
    }
    printf("\n");
}

void print(int *seq)
{
    int k = 0;
    while (seq[k] != 256)
        printf("%d ", seq[k++]);
}

int len(int *seq)
{
    int i = 0;
    while (seq[i++] != 256);
    return i - 1;
}