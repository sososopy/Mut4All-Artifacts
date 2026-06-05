
struct X // movable but not copyable
{
    X() = default;
    X(X &&) = default;

    X(const X &) = delete;
};

X non_parenthesized()
{
    X x;
    return x; // works
}

X parenthesized()
{
    X x;
    return (x); // error: use of deleted function 'X::X(const X&)'
}
