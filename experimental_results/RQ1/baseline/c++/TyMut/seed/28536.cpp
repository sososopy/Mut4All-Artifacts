
struct f
{
    ~f();
};

int foo(int t) {
    f g;
    switch (t) {
        case 1: return 1;
    }
    if (true)
        throw 1;
}
