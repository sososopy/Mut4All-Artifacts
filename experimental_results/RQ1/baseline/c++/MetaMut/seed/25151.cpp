
class Outer
{
public:
    class Inner
    {
    public:
        Inner() = default;
        ~Inner() {}  // user-defined, not default
    private:
        Inner(Inner&&) = default;
        friend class Outer; // this isn't being honored
    };
    Inner inners[1] = {};
};

int main()
{
    Outer v;
}
