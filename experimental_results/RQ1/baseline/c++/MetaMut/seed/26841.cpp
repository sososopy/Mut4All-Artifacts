

#include <vector>

class A
{
        int f(int a) const;
        int g() const;
};

int A::f(int a) const
{
        return 0;
}

int A::g() const
{
        const auto r = [&] (const auto begin, const auto end) {
                for(auto iter = begin; iter != end; ++iter) {
                        const auto newV = f(*iter);
                }
        };
        std::vector<int> p;
        r(p.begin(), p.end());
}

