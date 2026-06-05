
#include <cstdio>

template<typename T>
struct Somewhat {
private:
    void internal() const
    { printf("internal called\n"); }
    
public:
    friend void operator+(int const &, Somewhat<T> const &)
    {} // notice the in - class scope definition
};

void operator+(int const &, Somewhat<char> const &x)
{ x.internal(); }

int main() {
    // internal() called, the friend in - class *definition* ignored.
    1 + Somewhat<char>();
    return 0;
}

