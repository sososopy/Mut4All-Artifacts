
#include <fstream>

struct foo
{
    foo( std::fstream& stream )
        : stream_{ stream } // <-- Here's the problem
    {}

    std::fstream& stream_;
};

int main()
{
    std::fstream stream;
    foo f{ stream };
}

