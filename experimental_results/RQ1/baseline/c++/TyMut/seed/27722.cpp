
struct B {};
struct A {
    [[no_unique_address]] B a;
    [[no_unique_address]] B b;
    [[no_unique_address]] B c;
    [[no_unique_address]] B d;
};

int f() {
    return sizeof(A);
}

f():
        push    rbp
        mov     rbp, rsp
        mov     eax, 0
        pop     rbp
        ret

