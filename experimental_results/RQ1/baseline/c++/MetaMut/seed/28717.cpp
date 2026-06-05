
#include <arm_sve.h>
template <int N>
void f()
{
    int i = 0;
    static svbool_t pg = svwhilelt_b64(0, N);
}

int main(int argc, char **argv)
{
    f<2>();
    return 0;
}
