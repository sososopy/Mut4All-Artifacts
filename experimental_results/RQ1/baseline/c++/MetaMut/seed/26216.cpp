
struct A {
    A( int ) {}
};

struct B : virtual A 
{
    using A::A;
};

int main() {
    B b(5);
}

