
struct [[maybe_unused]] A {
    char a[5];
};

int main() {
    A x = { .a = "Hey!" }; // OK
    A y = { .a = { 'H', 'i', '\0' } }; // OK
    A z = { .a = "Hi" }; // error: C99 designator 'a' outside aggregate initializer
    return 0;
}
