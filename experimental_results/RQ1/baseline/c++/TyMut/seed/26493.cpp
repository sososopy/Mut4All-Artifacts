
#include <cstdio>

int main() {
    int i;
    try {
        try {
            throw (int*)0;
        } catch (int*& p) {
            p = &i;
            throw;
        }
    }
    catch (int*& p) {
        printf("%p", (void*)p);
    }
}

