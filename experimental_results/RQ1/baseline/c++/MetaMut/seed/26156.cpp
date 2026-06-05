#include <iostream>
#include <functional>
#include <vector>

template<class T>
class Surround {
public:
    class A;
    void dostuff();
};

template<class T>
class Surround<T>::A {
public:
    enum B { C, D };
    
    void dumpTokens(
        std::vector<int> input,
        std::function<B(int in)> choose
    ) {
        for(int x : input) {
            if(choose(x) == C)
                std::cout << 'C';
            else
                std::cout << 'D';
        }
    }
};

template<class T>
void Surround<T>::dostuff()
{
    std::vector<int> input;
    input.push_back(0);
    input.push_back(1);
    input.push_back(2);
    input.push_back(3);
    
    A a;
    
    // the following line causes an error
    A::B val = A::C;
    // It produces the error:
    //  error: missing 'typename' prior to dependent type name
    // The following line fixes the problem
    typename A::B val1 = A::C;
    
    // the following chunk of code causes an error
    a.dumpTokens(input, [](int x) -> A::B {
        return (x%2 == 0)? A::C : A::D;
    });
    // It produces the errors:
    //  error: type name requires a specifier or qualifier
    //  error: C++ requires a type specifier for all declarations
    //  error: expected body of lambda expression
    // The following version fixes the problem
    a.dumpTokens(input, [](int x) -> typename A::B {
        return (x%2 == 0)? A::C : A::D;
    });
    
    // ideally, the second chunk of code would produce an error
    // more like the first chunk of code
}

int main()
{
    Surround<bool> surround;
    surround.dostuff();
    return 0;
}