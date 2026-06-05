
#include <random>

int main ()
{
    std::mt19937 eng;
    std::normal_distribution<> rnorm;
    _Cilk_for (int i = 0; i != 10000; ++ i)
        rnorm(eng);
}

