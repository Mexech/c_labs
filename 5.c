#include <stdio.h>

#define N 10

void main() {
    int a[N] = { 0 }, s_even = 0, s_odd = 0;
    for (int i = 0; i < N; i++)
        scanf("%d", &a[i]);
    for (int i = 0; i < N; i++) {
        if (a[i] % 2 == 0) {
            s_even += a[i];
        } else {
            s_odd += a[i];
        }
    }
    if (s_even > s_odd) {
        printf("Sum of evens: %d", s_even);
    } else {
        printf("Sum if odds: %d", s_odd);
    }
}