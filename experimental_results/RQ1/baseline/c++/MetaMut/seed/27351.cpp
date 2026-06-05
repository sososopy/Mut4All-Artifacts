
#include <cstdio>

struct S {
    S(char const* message) {
        puts(message);
    }
};
void operator,(S,S){}

int main() {
    S("first"), S("second");

    operator,(S("FIRST"), S("SECOND"));
}
