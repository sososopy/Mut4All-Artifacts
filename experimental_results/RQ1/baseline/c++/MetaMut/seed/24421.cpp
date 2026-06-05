
struct Child
{
    Child (int);
    ~Child ();
    Child (const Child &) = delete;
};

struct Parent
{
    Parent () : children {{5}, {7}} {}

    Child children[2];
};
