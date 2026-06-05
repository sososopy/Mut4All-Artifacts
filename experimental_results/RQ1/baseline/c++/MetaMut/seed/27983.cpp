
#include <new>

int f()
{
    int n = -1;
    try {
        int *p = new(std::nothrow) int[n];
        if (!p) return 0;
    } catch (const std::bad_array_new_length&) {
        return 1;
    }

    return 2;
}

