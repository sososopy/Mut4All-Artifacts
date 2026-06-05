
#include <stdio.h>

int f(int &r) {
   r = r + 1;
   return r;
}

int main(void) {

    int j = 99;
    int i = 99;

    printf("i = %d\n",i);
    printf("f(i) = %d\n",f(i));
    printf("i = %d\n",i);

    return 0;
} 

