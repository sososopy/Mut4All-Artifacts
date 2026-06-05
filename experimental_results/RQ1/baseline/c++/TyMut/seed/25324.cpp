

constexpr bool checkForPrimeNumber(const int p, const int t)
{
    return p <= t or (p % t and checkForPrimeNumber(p, t + 2));
}

constexpr bool checkForPrimeNumber(const int p)
{
    return p == 2 or (p & 1 and checkForPrimeNumber(p, 3));
}

int main() 
{
    static_assert(checkForPrimeNumber(65521), "bug...");
}


