#include <stdio.h>
void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}
int main(int argc, char *argv[]){
    int a = 3;
    int b = 4;
    printf("a is %d and b is %d\n", a, b);
    swap(&a, &b);
    printf("a is now %d and b is now %d\n", a, b);
    return 0;
}

