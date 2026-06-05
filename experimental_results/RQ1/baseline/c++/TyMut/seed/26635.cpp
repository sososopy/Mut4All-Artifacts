

struct Bob
{
    explicit Bob() {}
};

Bob f() 
{
    return {}; // #1
}

void f2(Bob) {}

int main()
{
    f2({}); // #2
}

