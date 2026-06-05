
struct base {
    explicit constexpr base(int&&) {}
};

struct derived: base {
    using base::base;
};

int main()
{
    derived d { 0 };
}
