
unsigned long long s(unsigned long long n)
{
    auto s = 0ULL;
    for (auto i = 0ULL; i < n; i++)
        s += i;
    return s;
}

const auto N = 1000000000ULL;

auto x = s(N); // It computes in run-time and dose NOT compute in compile-time!
