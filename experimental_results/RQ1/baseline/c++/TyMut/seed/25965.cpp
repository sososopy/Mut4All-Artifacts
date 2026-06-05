
extern "C" int puts(char const*);
struct B {
    ~B() { puts("\t~B"); }
};
struct A {
    static B make() { return B(); }
} a;
A *ap() { return &a; }
int main () {
    puts("b1");
    {B b = A::make();}
    puts("b2");
    {B B = a.make();}
    puts("b3");
    {B b = ap()->make();}
    puts("b4");
    {B b = A().make();}
}
