#define MAX_STRING_SIZE 1000

int is_delimiter(int c)
{
    return c == ' ' || c == '.' || c == ',' || c == '\n';
}

void main() {
    int i, len, i_prev, j;
    char result[MAX_STRING_SIZE] = {0};
    while ((*(result + j) = getchar()) != '\n' && j < MAX_STRING_SIZE)
    {
        if (is_delimiter(*(result + j - 1)) && !is_delimiter(*(result + j)))
        {
            i_prev = i;
            i = j - 1;
        }
        j++;
    }
    len = j;
    if (i) 
        for (j = i_prev; j <= len; j++){
            *(result + j) = *(result + j + i - i_prev);
            *(result + j + i - i_prev) = 0;
        }
    printf(result);
}