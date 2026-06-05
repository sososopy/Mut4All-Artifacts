
#include <type_traits>

template <typename... Args>
void f(int &&a, Args&&... args)
{
}

template <typename K, typename... Args, typename = typename std::enable_if<true, K>::type>
void f(K&& k, Args&&... args)
{
}

int main() {
    f(1, 2);
    return 0;
}

