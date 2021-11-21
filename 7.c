#include <stdio.h>
 
void showbits(unsigned int x) {
    int i = 0;
    for (i = (sizeof(int) * 8) - 1; i >= 0; i--)
    {
       putchar(x & (1 << i) ? '1' : '0');
    }
    printf("\n");
}
 
void main() {
    long int n, left_ptr_bit, right_ptr_bit = 1, i = 0, is_negative = 0;
    scanf("%d", &n);
    showbits(n);
    if (n < 0) {
        n = ~n;
        is_negative = 1;
    }
    for (i = (sizeof(int) * 8) - 1; i >= 0; i--) {
        if (n & (1 << i)) {
            left_ptr_bit = 1 << i;
            break;
        }
    }
    while (left_ptr_bit > right_ptr_bit) {
        if (((n & right_ptr_bit) && right_ptr_bit) != ((n & left_ptr_bit) && left_ptr_bit)) {
            n ^= left_ptr_bit | right_ptr_bit;
        }
        left_ptr_bit = left_ptr_bit >> 1;
        right_ptr_bit = right_ptr_bit << 1;
    }
    if (is_negative) n = ~n;
    showbits(n);
}
