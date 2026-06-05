
struct B { };
struct D : B {
    int x;
    int y;
};

int main(int, char**) {
    D d = {.x=1, .y=2};
    (void)d;
}
