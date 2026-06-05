

#include <string>
#include <iostream>

void Test1(const bool arg) 
{
    std::cout << "Right[" << arg << "] (standard conversion sequence)" << std::endl;
}
void Test1(const std::string arg)
{
    std::cout << "Wrong[" << arg.size() << "] (user-defined conversion sequence)" << std::endl;
}

void Test2(const int arg) 
{
    std::cout << "Wrong[" << arg << "] (no conversion, doesn't initializes std::initializer_list)" << std::endl;
}
void Test2(const std::initializer_list<int> arg)
{
    std::cout << "Right[" << arg.size() << "] (no conversion, initializes std::initializer_list)" << std::endl;
}

struct S 
{ 
    S(int _a) : a(_a){}
    int getA() const { return a; }
private:
    int a;
};
void Test3(const int arg) 
{
    std::cout << "Right[" << arg << "] (standard conversion sequence)" << std::endl;
}
void Test3(const S arg)
{
    std::cout << "Wrong[" << arg.getA() << "] (user-defined conversion sequence)" << std::endl;
}

void Test4(const bool arg) 
{
    std::cout << "Right[" << arg << "] (standard conversion sequence)" << std::endl;
}
void Test4(const std::initializer_list<std::string> arg)
{
    std::cout << "Wrong[" << arg.size() << "] (user-defined conversion sequence)" << std::endl;
}

int main (int /*argc*/, char * const /*argv*/[]) 
{
    Test1({"false"});
    Test2({123});
    Test3({456});
    Test4({"false"});
    return 0;
}

