#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int size;
    int *num;

    printf("Enter a number between 1 and 50: ");
    scanf("%d", &size);

    if (size<1 || size> 50){
        printf("number between 1 and 50.\n");
        return 1;
    }

    num= (int *)malloc(size *sizeof(int));
    if (num == NULL){
        printf("memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i <size; i++){
        num[i] = rand();
        printf("element %d is %d\n", i, num[i]);
    }
    free(num);
    return 0;
}

