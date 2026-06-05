

struct A {};

struct B
{
    A a;
    friend decltype(a); // This works, B is not a class template
};

template <typename T>
struct C
{
    A a;
    // This friend declaration gives an ICE.
    // Seemingly because C is a class template
    friend decltype(a); 
};

int main()
{
    B b;
    C<int> c;   // This causes the ICE
}

