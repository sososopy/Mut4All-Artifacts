
typedef __attribute__((aligned(1))) int packed_int;

struct __attribute__((packed)) Foo {
    int i;
    packed_int& get() { return i; }
};

