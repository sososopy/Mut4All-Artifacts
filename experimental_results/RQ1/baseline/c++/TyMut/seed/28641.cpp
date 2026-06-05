
void f(bool) {}

int main() {
    int x;
    f(requires {x++;});
}
