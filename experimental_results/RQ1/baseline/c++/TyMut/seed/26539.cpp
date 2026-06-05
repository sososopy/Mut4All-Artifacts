
#include <stdio.h>
int main() {
    try {
        throw nullptr;
    } catch (const int(&)[2]) {
        puts("caught int(&)[2]");
    } catch (const int*) {
        puts("caught int*");
    }
}

