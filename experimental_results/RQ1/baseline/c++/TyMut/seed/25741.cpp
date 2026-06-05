
#include <iostream>

class rva
{
public:
    rva() { std::cout << "Default construction\n"; }
    rva(rva const&) { std::cout << "Copy construction\n"; }
    rva& operator=(rva const&) { std::cout << "Assignation\n"; }
    ~rva() { std::cout << "Destruction\n"; }
};

rva f(int i) {
    if (i == 0) {
        rva result;
        return result;
    } else {
        return rva();
    }
}

int main()
{
    { std::cout << "f(0)\n"; f(0); }
    { std::cout << "\nf(1)\n"; f(1); }
    { std::cout << "\ng(0)\n"; g(0); }
    { std::cout << "\ng(1)\n"; g(1); }
    return 0;
}
