
#include <vector>

template<typename Value>
struct A
{
    A() = default;
    std::vector<Value> m_content;
};

void func(A<int> a = {})
{
}

int main()
{
    func();
}
