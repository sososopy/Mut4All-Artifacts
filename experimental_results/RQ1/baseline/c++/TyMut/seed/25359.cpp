
#include<algorithm>

struct foo
{
    static constexpr int bar=100;
    
    int operator()() const
    {
        return std::max(bar, bar);//got an error here
    }
} F;

int main()
{
    F();
}
