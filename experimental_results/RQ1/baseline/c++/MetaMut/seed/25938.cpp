
#include <functional>

int main(int argc, char *argv[])
{
    std::function<int ()> f = []{ return 0; };
    return 0;
}

