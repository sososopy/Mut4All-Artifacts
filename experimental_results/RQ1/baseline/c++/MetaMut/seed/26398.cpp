
#include <stdexcept>

__attribute__((target_clones("default,avx2")))
void foo()
{
    throw std::runtime_error("");
}

int main()
{
    try { foo(); } catch (...) { }
}

