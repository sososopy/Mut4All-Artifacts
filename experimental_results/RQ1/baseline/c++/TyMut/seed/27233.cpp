
#include <cstdio>

int glob = 1;

bool func(int i)
{
    printf("setting glob to %d.. \n", i);
    glob = i;
}  // missing return statement


int main()
{
    func(10);  // return value is not used!
    printf("cleanly exiting..\n");
    fflush(stdout);
}
