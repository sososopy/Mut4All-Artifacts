
#ifdef IS_INCLUDE_FILE
#pragma clang system_header

#define FOO(a) ((int*) (a))

#else
#define IS_INCLUDE_FILE
#include __FILE__

int main()
{
    int* i = FOO(0);
}
#endif

