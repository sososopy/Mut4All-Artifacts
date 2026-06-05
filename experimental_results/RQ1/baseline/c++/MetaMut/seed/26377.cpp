
struct A { constexpr static int a = 0; };
struct B : A {};
struct C : A {};
struct D : B, C {};

int main() {
    (void)D{}.a;   //ok everywhere
    (void)D{}.A::a; //error in GCC
}

