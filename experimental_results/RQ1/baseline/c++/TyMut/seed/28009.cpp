
#include <string>
#include <functional>
#include <iostream>

using namespace std;

class S {
    public:
        using ConverterFunction = function<double(double)>;

        void _foo(
                const ConverterFunction& converter = [](double value) -> double { return value; },
                const ConverterFunction& converterBack = [](double value) -> double { return value; }
                );
};


void S::_foo(
        const ConverterFunction& converter,
        const ConverterFunction& converterBack)
{
    cout << "foo " << converter(42) << endl;
}

int main() {
    S s;
    s._foo();
    return 0;
}

