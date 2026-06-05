
#include <iostream>
#include <utility>
class foo{
    public:
        foo() = default;

        foo(const foo &fv):iv(fv.iv){std::cout << "foo copy constructor\n";}
        foo &operator=(foo &fv)
        {
            std::cout << "foo copy assignment\n";
            iv = fv.iv;
            return *this;
        }
        ~foo()
        {
            std::cout << "foo destructor\n";
        }
    private:
        int iv;
};

class test
{
    public:
        test() = default;
        test(const test &tv):iv(tv.iv){
            std::cout << "test copy constructor\n";
        }

        test &operator=(const test &fv)
        {
            std::cout << "test copy assignment\n"; 
            iv = fv.iv;
            return *this;
        }
        test(test &&) = default;
        test &operator=(test &&) = default;
    private:
        int iv;
        foo fv;
};

int main()
{
    test ta;
    test tb(std::move(ta));
    ta = std::move(tb);
    return 0;
}

