
struct X
{
    ~X() {}
};

union U
{
    X x;
    ~U() {}
};

U u;

