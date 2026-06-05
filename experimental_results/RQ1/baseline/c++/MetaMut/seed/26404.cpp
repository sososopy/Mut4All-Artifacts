
int global_x{};

struct ConvToRef {
    operator int&() { return global_x; }
};

struct Foo { int& r; };

int main()
{
    Foo bar{ { ConvToRef{} } };
}
