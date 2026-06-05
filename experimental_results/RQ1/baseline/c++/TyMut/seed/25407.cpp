
struct X
{
    X (int);
    X (const X&) = delete;
};

X arrX = { 1, 2, 3 };

