class Single
{
private:
    Single() = default;
};

auto s1 = Single{}; // compiles fine with c++11,14,17. Compilation error with c++20. Unexpected.
auto s2 = Single(); // compilation error. expected behavior.
Single s3;          // compilation error. expected behavior.
