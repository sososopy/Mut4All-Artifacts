
#include <stdexcept>
#include <iostream>

struct useless
{
    useless() { std::cout << "in" << std::endl; }
    ~useless() { std::cout << "out" << std::endl; throw std::runtime_error("who knows..."); }
};

int main(int argc, char* argv[])
{
    useless u;
    return argc;
}

