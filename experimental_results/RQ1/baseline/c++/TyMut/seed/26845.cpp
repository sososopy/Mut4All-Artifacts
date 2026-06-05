
struct test
{
        // it only works if e is declared before test()
        test() noexcept(e) {}
        static const bool e = false;
};

int main() {}

