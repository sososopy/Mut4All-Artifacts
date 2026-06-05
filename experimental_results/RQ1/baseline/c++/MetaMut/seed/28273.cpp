
module;

#include <memory>

export module test_support;

export template <class T> struct pimpl
{
private:
   std::shared_ptr<T> data;
public:
   pimpl(const T& x) : data(new T(x)) {}
   pimpl() = default;
};



import test_support;

int main()
{
   pimpl<int> p1, p2(3);
}

