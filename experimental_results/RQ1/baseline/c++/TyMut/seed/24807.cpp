
struct A {};
struct B: public A {int stuff() {return 5;} };  
struct C: protected B { using B::A; };

void call(A &) {}

int main()
{
    C c; 
    call(c);
}
