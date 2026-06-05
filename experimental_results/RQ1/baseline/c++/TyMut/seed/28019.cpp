
#include <stdio.h>
int parseList()
{
       int i;
       for (i = 0; i < 3; i++) {
               printf("DUPA: %d %d\n", i, i);
       }
}
int main(int argc, char **argv)
{
       printf("start\n");
       parseList();
}
