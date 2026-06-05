
extern thread_local constinit int x;

int foo_good() {
    return x;
}

void set_foo(int y) {
    x = y;
}
