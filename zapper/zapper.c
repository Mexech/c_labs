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
#define MAX_DICT_LEN 10240
#define INIT_ALPH_LEN 257

FILE *arch;

int bit_counter = 0;
unsigned char cur_byte = 0;
int cur_byte_number = 0;
int dict_extended_len = 0;
int seq_len = 0;
int paths_len = 0;
int result_len = 0;
int i_m = 0;
int eocf = 0;
int eob = 0;
int dict_size = INIT_DICT_SIZE;
int total_sz = 0;
int buff_leftover = -1;

int **dict;
char **paths;

int in_dict(int *seq);
int dict_index(int *seq);
void lzw_encrypt(unsigned char *buff, FILE *from_file, int amount);
void lzw_encrypt_file(char *path);
char *lzw_decrypt(char *buff, FILE *from_arch, int amount);
void lzw_decrypt_file(char *path, FILE *from_arch);
void append_dict(int *seq_address);
void reset_dict();
void init_dict();
int streq(int *s1, int *s2);
void append(int *seq, int c);
void concat_r(char *res, int *seq);
void concat_s(int *seq1, int *seq2);
int len(int *seq);
char *origin_folder(char *path);
char *strremove(char *str, const char *sub);
int is_regular_file(const char *path);
int is_folder(const char *path);
void paths2file(char *origin);
void fill_paths(const char *name);
void process_paths(char *prefix);
char *without_filename(char *path);
void make_archive(char *path_to_encrypt, char *output_path);
void extract_archive(char *path_to_decrypt, char *output_path);
char *filename_no_ext(char *path);
char *prettify(char *path);
int count_bits(int number);
void write_as_bits(unsigned int number);
void pack(int code);
unsigned char read_next_bit(char *buff);
int unpack(char *buff);

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    paths = malloc(PATHS_AMOUNT * sizeof(char *));
    if (argc > 1)
    {
        char *extension = calloc(4, 1);
        char *output_path = calloc(PATH_SIZE, 1);
        if (!is_folder(argv[1]) && !is_regular_file(argv[1]))
        {
            printf("Input path does not exist");
            exit(0);
        }
        if (argc == 3)
        {
            if (is_regular_file(argv[2]))
            {
                printf("Output folder path cannot be path to file\n");
                exit(0);
            }
            else if (!is_folder(argv[2]))
            {
                char c;
                printf("Output folder cannot be found, create it?(Y/N)\n");
                scanf("%c", &c);
                if (c == 'Y' || c == 'y')
                {
                    char *command = calloc(PATH_SIZE, 1);
                    sprintf(command, "mkdir \"%s\"", argv[2]);
                    system(command);
                    if (!is_folder(argv[2]))
                        exit(0);
                }
                else if (c == 'N' || c == 'n')
                    exit(0);
            }
            strcpy(output_path, argv[2]);
        }
        else
            strcpy(output_path, without_filename(argv[1]));
        sprintf(extension, "%.*s", 4, argv[1] + strlen(argv[1]) - 4);
        if (!strcmp(extension, ".zap"))
        {
            extract_archive(argv[1], output_path);
        }
        else
        {
            sprintf(output_path, "%s/%s.zap", output_path, filename_no_ext(argv[1]));
            while (access(output_path, F_OK) == 0)
            {
                char c;
                printf("%s -- already exists, overwrite?(Y/N)\n", prettify(output_path));
                scanf("%c", &c);
                if (c == 'N' || c == 'n')
                    sprintf(output_path, "%s/%s-(2).zap", argc == 3 ? without_filename(argv[2]) : without_filename(argv[1]), filename_no_ext(argv[1]));
                else if (c == 'Y' || c == 'y')
                    break;
            }
            make_archive(argv[1], output_path);
        }
    }
    return 0;
}

char *prettify(char *path)
{
    char *result = calloc(PATH_SIZE, 1);
    char c, prev_c, i = 0, j = 0, found_slash = 0;
    while ((c = path[i++]) != 0)
        if (c != '\\' && c != '/')
        {
            result[j++] = c;
            found_slash = 0;
        }
        else if (!found_slash)
        {
            result[j++] = '\\';
            found_slash = 1;
        }
    return result;
}

char *origin_folder(char *path)
{
    char *origin = calloc(PATH_SIZE, 1);
    int i = strlen(path), j = 0;
    while (((origin[j++] = path[--i]) != '/') && (origin[j - 1] != '\\'))
        ;
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

int is_folder(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
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
            paths[paths_len++] = path;
        }
        else
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
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
    while (result[j--] = 0, (result[j] != '/') && (result[j] != '\\'))
        ;
    for (int i = 0; i < strlen(result); i++)
        if (result[i] == '/')
            result[i] = '\\';
    return result;
}

void make_archive(char *path_to_encrypt, char *output_path)
{
    if (is_regular_file(path_to_encrypt))
        paths[paths_len++] = path_to_encrypt;
    else
        fill_paths(path_to_encrypt);
    init_dict();

    arch = fopen(output_path, "wb+");
    fseek(arch, 0, SEEK_END);
    total_sz = ftell(arch);
    fseek(arch, 0, SEEK_SET);
    paths2file(path_to_encrypt);
    lzw_encrypt_file("paths.tmp");
    remove("paths.tmp");
    for (int i = 0; i < paths_len; i++)
    {
        lzw_encrypt_file(paths[i]);
        free(paths[i]);
    }
    if (bit_counter > 0)
    {
        cur_byte <<= 8 - bit_counter;
        fwrite(&cur_byte, 1, 1, arch);
        bit_counter = 0;
    }
    free(paths);
    paths = malloc(PATHS_AMOUNT * sizeof(char *));
    paths_len = 0;
    fclose(arch);
}

void extract_archive(char *path_to_decrypt, char *output_path)
{
    init_dict();
    arch = fopen(path_to_decrypt, "rb+");
    if (arch == NULL)
        perror("nonexistant output file");
    fseek(arch, 0, SEEK_END);
    total_sz = ftell(arch);
    fseek(arch, 0, SEEK_SET);
    lzw_decrypt_file("paths.tmp", arch);
    process_paths(output_path);
    for (int i = 0; i < paths_len; i++)
    {
        lzw_decrypt_file(paths[i], arch);
        free(paths[i]);
    }
    free(paths);
    paths_len = 0;
}

char *filename_no_ext(char *path)
{
    char *result = calloc(_MAX_PATH, 1);
    strcpy(result, path);
    strremove(result, without_filename(result));
    if (is_folder(path))
        return result;
    else
    {
        int i = strlen(result);
        while (--i != 0)
            if (result[i] == '.')
            {
                result[i] = 0;
                break;
            }
            else if (result[i] == '\\' || result[i] == '/')
                break;
        return result;
    }
}

void write_bit(unsigned char bit)
{
    cur_byte <<= 1;
    cur_byte |= bit;
    if (++bit_counter == 8)
    {
        fwrite(&cur_byte, 1, 1, arch);
        bit_counter = 0;
        cur_byte = 0;
    }
}

int count_bits(int number)
{
    int i = 0;
    while (i++, (number >>= 1) != 0);
    return i;
}

void write_as_bits(unsigned int number)
{
    if (number >> 1)
    {
        write_as_bits(number >> 1);
    }
    write_bit(number & 1);
}

void pack(int code)
{
    int prefix_zeros = count_bits(INIT_ALPH_LEN + dict_extended_len + 1) - count_bits(code);
    for (int i = 0; i < prefix_zeros; i++)
    {
        write_bit(0);
    }
    write_as_bits(code);
}

unsigned char read_next_bit(char *buff)
{
    int bit = ((buff[cur_byte_number] << bit_counter++) & 128) >> 7;
    if (bit_counter == 8)
    {
        if (cur_byte_number == BUFF_SIZE - 2)
        {
            buff[cur_byte_number] = fgetc(arch);
            eob = 1;
        }
        else
            cur_byte_number++;
        bit_counter = 0;
    }
    return bit;
}

int unpack(char *buff)
{
    int result = 0;
    int bits_to_read = count_bits(INIT_ALPH_LEN + dict_extended_len + 2);
    for (int i = bits_to_read - 1; i >= 0; i--)
    {
        result += (1 << i) * read_next_bit(buff);
    }
    if (result == 256)
    {
        reset_dict();
        result = unpack(buff);
    }
    if (cur_byte_number == BUFF_SIZE - 2)
        buff_leftover = result;
    return result;
}

void lzw_encrypt_file(char *path)
{
    unsigned char buff[BUFF_SIZE] = {0};
    FILE *in = fopen(path, "rb");
    fseek(in, 0L, SEEK_END);
    int total_sz = ftell(in);
    fseek(in, 0, SEEK_SET);
    if (total_sz == 0)
        pack(257);
    if (in == NULL)
        perror("nonexistant input file");
    if (strcmp(path, "paths.tmp"))
        printf("Compressing %s\n", prettify(path));
    do
    {
        size_t n = fread(buff, sizeof(buff[0]), sizeof(buff), in);
        eocf = 0;
        if (n)
        {
            lzw_encrypt(buff, in, n);
        }
        else if (n < BUFF_SIZE)
        {
            pack(257);
            break;
        }
        if (strcmp(path, "paths.tmp"))
            printf("%.2f%%\n", ((float)ftell(in) / total_sz) * 100);

    } while (1);
    fclose(in);
    reset_dict();
}

void lzw_encrypt(unsigned char *buff, FILE *from_file, int amount)
{
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
                pack(dict_index(seq));
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
}

void lzw_decrypt_file(char *path, FILE *from_arch)
{
    char buff[BUFF_SIZE] = {0};
    eocf = 0;
    if (paths_len) 
    {
        char *command = calloc(PATH_SIZE, 1);
        strcpy(command, "mkdir \"");
        strcat(command, without_filename(path));
        strcat(command, "\" >nul 2>nul");
        system(command);
    }
    FILE *out = fopen(path, "wb+");
    if (out == NULL)
        perror("nonexistant input file");
    do
    {
        size_t n = fread(buff, sizeof(buff[0]), BUFF_SIZE - 1, from_arch);
        cur_byte_number = 0;
        eob = 0;
        if (n)
        {           
            char *message = lzw_decrypt(buff, from_arch, n);
            fwrite(message, sizeof(message[0]), result_len, out);
            free(message);
        }
        else
            break;
        printf("%.2f%%\n", ((float) ftell(from_arch) / total_sz) * 100);
    } while (!eocf);
    fclose(out);
    reset_dict();
}

void concat_r(char *res, int *seq)
{
    int i;
    for (i = 0; seq[i] != 256; i++)
        res[result_len++] = seq[i];
}

void concat_s(int *seq1, int *seq2)
{
    int l1 = len(seq1);
    for (int i = 0; seq2[i] != 256; i++)
        seq1[l1++] = seq2[i];
    seq1[l1] = 256;
    seq_len = l1;
}

char *lzw_decrypt(char *buff, FILE *from_arch, int amount) {
    int result_cap = RESULT_SIZE;
    result_len = 0;
    char *result = calloc(result_cap, sizeof(char));
    int k = 0, code = 0;
    if (dict[INIT_ALPH_LEN + 1] == NULL) // checking wether decrypting first batch
    {
        code = unpack(buff);
        if (code == 257) // if file empty
        {
            eocf = 1;
            fseek(from_arch, -(amount - cur_byte_number) * sizeof(buff[0]), SEEK_CUR);
        } 
        else
            concat_r(result, dict[code]);
    } 
    while (cur_byte_number < amount && cur_byte_number < BUFF_SIZE && !eocf && !eob)
    {
        int *seq = malloc(sizeof(int) * BUFF_SIZE);
        seq_len = 0; seq[seq_len] = 256; k = 0;
        if (buff_leftover != -1)
        {
            code = buff_leftover;
            buff_leftover = -1;
        }
        concat_s(seq, dict[code]);
        code = unpack(buff);
        while (code != 257)
        {
            if (!dict[code])
                append(seq, seq[0]);
            else
                append(seq, dict[code][k++]);
            if (!in_dict(seq))
            {
                append_dict(seq);
                if (result_len + len(dict[code]) + 1 >= result_cap)
                {
                    result_cap *= 1.5;
                    result = realloc(result, result_cap);
                }
                concat_r(result, dict[code]);
                break;
            }
            else if (dict[code][k] == 256)
                break;
        }
        if (code == 257)
        {
            eocf = 1;
            if (amount > cur_byte_number)
            {
                fseek(from_arch, -(amount - cur_byte_number)*sizeof(buff[0]), SEEK_CUR);
            }
        }
    }
    if (eob)
        fseek(from_arch, -sizeof(buff[0]), SEEK_CUR);
    return result;
}

void reset_dict()
{
    for (int i = 0; i < INIT_ALPH_LEN + dict_extended_len + 1; i++)
        if ((i != 256) && (i != 257))
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

void append_dict(int *seq_address)
{
    if (INIT_ALPH_LEN + dict_extended_len + 2 >= MAX_DICT_LEN)
    {
        if (!cur_byte_number)
            pack(256);
        reset_dict();
    } 
    else if (INIT_ALPH_LEN + dict_extended_len + 2 >= dict_size)
    {
        dict_size *= 1.5;
        dict = realloc(dict, dict_size * sizeof(int *));
    }
    dict[INIT_ALPH_LEN + 1 + dict_extended_len++] = seq_address;
    dict[INIT_ALPH_LEN + 1 + dict_extended_len] = NULL;
}

int in_dict(int *seq)
{
    for (int k = INIT_ALPH_LEN + dict_extended_len; k >= 0; k--)
        if ((k != 256) && (k != 257)) 
            if (streq(dict[k], seq))
                return 1;
    return 0;
}

int dict_index(int *seq)
{
    for (int k = INIT_ALPH_LEN + dict_extended_len; k >= 0; k--)
        if ((k != 256) && (k != 257))
            if (streq(dict[k], seq))
                return k;
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

int len(int *seq)
{
    int i = 0;
    while (seq[i++] != 256);
    return i - 1;
}