
struct Inner
{
    int i = 0;
};

struct Outer2
{
    Inner s{};
};

template <typename T = int>
struct Outer
{
    Inner s{};
};

int main()
{
    Outer2 o21{ .s = {} };                // works
    Outer2 o22{ .s = Inner{ .i = 1} };    // works
    Outer2 o23{ .s = { .i = 1} };         // works

    Outer2 o24{ .s{} };                   // works
    Outer2 o25{ .s{Inner{ .i = 1} } };    // works
    Outer2 o26{ .s{ .i = 1} };            // works

    Outer o1{ .s = {} };                // works
    Outer o2{ .s = Inner{ .i = 1} };    // works
//    Outer o3{ .s = { .i = 1} };         // does not

    Outer o4{ .s{} };                   // works
    Outer o5{ .s{Inner{ .i = 1} } };    // works
//    Outer o6{ .s{ .i = 1} };            // does not
}
