
struct blank {};

template <typename T>
struct result_builder
{
    template <typename... Blanks>
    void apply(Blanks... blanks);
};

template <typename T>
void waldo()
{
    result_builder<T> r;
    typedef decltype(r.apply(blank{})) wally;
};

int main()
{
    waldo<int>();
}


