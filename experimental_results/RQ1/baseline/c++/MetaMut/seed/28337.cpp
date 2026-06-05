
struct C
{
    int x = 0;
};

struct S
{
    C c;
    bool flag = false;

};

S test()
{
    return {.flag = true};
}

