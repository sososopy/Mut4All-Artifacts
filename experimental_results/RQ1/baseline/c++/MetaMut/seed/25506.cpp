
template <typename T, int x, int y>
struct M;

template <>
struct M<float, 3, 3>
{
    static M<float, 3, 3> bar()
    {
        return {};
    }
};

typedef M<float, 3, 3> M3f;

struct Foo
{
    //M<float, 3, 3> m = M3f::bar(); // Works
    M<float, 3, 3> m = M<float, 3, 3>::bar(); // Does not work
};

int main() {}
