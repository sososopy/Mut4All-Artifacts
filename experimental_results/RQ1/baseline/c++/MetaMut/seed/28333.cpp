#include <iostream>

using namespace std;

class Block
{
public:
    Block(int n) : data{new char[n]}, size{n}
    {
        cout << "Block ctor\n";
    }

    ~Block()
    {
        cout << "Block dtor\n";
        delete[] data;
    }

private:
    char* data;
    int size;
};

struct Cargo
{
    Block const& block;
};

int main()
{
    {
        Cargo* c = new Cargo{{4000}};
        cout << "main\n";
        delete c;
    }
    return 0;
}

