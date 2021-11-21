#include<stdio.h> 
#include<math.h> 
void main(){  
    double pi = acos(-1);
    int r, a;
    scanf("%d %d", &r, &a);
    double area = (pi * r * r)-(a * a);
    printf("%f", area);
}