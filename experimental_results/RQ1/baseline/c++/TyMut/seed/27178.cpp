
#include <stdio.h>

int main() {
    alignas(32) double d;
    double &ref_d = d;

    printf("Alignment of d: %d\n", alignof(d));             // 32
    printf("Alignment of ref_d: %d\n", alignof(ref_d));     // 8
}

