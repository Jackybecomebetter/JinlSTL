#include <stdio.h>
#include <stdlib.h>
// #include <iostream>
// #include <vector>
// #include <algorithm>

// using std::cout;
// using std::endl;
int main()
{
    
    int *p[5] = {0,0,0,0,0};
    int ** a;
    *p = (int *)malloc(sizeof(int));
    *(p+1) = (int *)malloc(sizeof(int));
    *(p+2) = (int *)malloc(sizeof(int));
    *(p+3) = (int *)malloc(sizeof(int));
    *(p+4) = (int *)malloc(sizeof(int));

    **p = 1;
    a = p;
    **(a+4) = 3;
    printf("p 3 addr is %d\n",*(p+3));
    printf("p 4 addr is %d\n",*(p+4));
}