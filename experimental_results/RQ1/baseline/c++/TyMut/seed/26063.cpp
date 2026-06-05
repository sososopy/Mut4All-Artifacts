
class A {
    static int p;
};
int A::p = 0;
template<int=0>
struct B : A {
    B() {(void)p;}
};
int main() {
    B<>();
}
