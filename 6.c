#include <stdio.h>
#define K 3
#define N 4

void printMat(int a[K][N]) {
    int i, j;
    for( i = 0; i < K; i++ ) {
        for( j = 0; j < N; j++ ) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void main() {
    int a[K][N], i, j;
    for( i = 0; i < K; i++ )
        for( j = 0; j < N; j++ )
            scanf("%d", &a[i][j]);
    int min = a[0][0], max = a[0][0]; 
    for( i = 0; i < K; i++ )
        for( j = 0; j < N; j++ )
            if (a[i][j] > max) {
                max = a[i][j];
            } else if (a[i][j] < min) {
                min = a[i][j];
            }

    for( i = 0; i < K; i++ )
        for( j = 0; j < N; j++ )
            if (a[i][j] < 0) {
                a[i][j] = min;
            } else {
                a[i][j] = max;
            }
    printMat(a);
}