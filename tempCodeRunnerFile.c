#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_SIZE 100
#define MAX_STRING_SIZE 1000
#define TRUE 1 
#define FALSE 0 

int is_delimiter(int c) {
    return c == ' ' || c == '.' || c == ',' || c == '\n';
}

int is_vowel(int c) {
    return c == 'a' || c == 'e' || c == 'i' ||  c == 'o' || c == 'u';
}

void main(){
    int c = ' ', prev_c, i = 0, j = 0, stopped_at = 0, has_double = FALSE;
    int word[MAX_WORD_SIZE] = { 0 };
    int result[MAX_STRING_SIZE] = { 0 };
    do {
        prev_c = c;
        c = getchar();
        if (!is_delimiter(c)) {
            do {
                if (is_vowel(tolower(prev_c)) && is_vowel(tolower(c))) {
                    has_double = TRUE;
                }
                word[i] = c;
                i++;
                prev_c = c;
            } while (!is_delimiter(c = getchar()));
        }
        if (has_double) {
            memset(word, 0, MAX_WORD_SIZE);
            has_double = FALSE;
        }
        i = 0;
        for (j = stopped_at; j < stopped_at + MAX_WORD_SIZE; j++) {
            if (word[i]) {
                result[j] = word[i];
                i++;
            } else {
                break;
            }
        }
        memset(word, 0, MAX_WORD_SIZE);
        i = 0;
        if (j != stopped_at) {
            result[j+1] = c;
            stopped_at = j + 2;
        } else {
            result[j] = c;
            stopped_at = j + 1;
        }
    } while(c != '\n' && j < MAX_STRING_SIZE);
    for(i = 0; i < MAX_STRING_SIZE; i++)
            printf("%c", result[i]);
}