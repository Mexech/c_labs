#include<stdio.h>
void main(){
    int x, n, i=0, j=1, fact=1, xPow, sign=1;
    double s = 0;
    scanf("%d %d", &x, &n);
    fact = 1;
    sign = 1;
    xPow = x;
    while (i < n){
        s += ((double)sign*((double)xPow/(double)fact));
        i += 1;
        j += 2;
        sign *= -1;
        fact *= (j-1)*j;
        xPow *= x*x;
    };
    printf("%f", s);
}
