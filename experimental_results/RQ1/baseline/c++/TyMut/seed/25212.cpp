#include <vector>

int main()
{
    std::vector<int> v[] = {std::vector<int>(10), std::vector<int>(10)};
    auto lambda = [v]{};
}

