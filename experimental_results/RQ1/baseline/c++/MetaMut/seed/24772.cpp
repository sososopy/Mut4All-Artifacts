// test-main.cc
struct S1 { consteval S1(int) {} };
int main() {
    struct S2 { S1 s = 0; };
    S2 s;
}