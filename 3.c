#include <stdio.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

void main(){
    int c, found_vowel = FALSE, counter = 0, is_vowel = FALSE, prev_c = ' ';
    while((c = tolower(getchar())) != '\n') {
        is_vowel = (c == 'a' || c == 'e' || c == 'i' ||  c == 'o' || c == 'u');
        if ((prev_c == ' ' || prev_c == '.' || prev_c == ',') && is_vowel) {
            found_vowel = TRUE;
        }
        if(c == ' ' || c == '.' || c == ',') {
            if (prev_c != 'a' && prev_c != 'e' && prev_c != 'i' && prev_c != 'o' && c != 'u' && found_vowel) {
                counter++;
            }
            found_vowel = FALSE;
        }
        prev_c = c;
    }
    if (prev_c != 'a' && prev_c != 'e' && prev_c != 'i' && prev_c != 'o' && c != 'u' && found_vowel) {
        counter++;
    }
    printf("Amount of words starting with vowel and ending with consonant: %d", counter);
}