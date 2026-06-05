
template <typename T>
void foo() = delete;

template <>
void foo<int>()
{
}

int main()
{
    foo<int>();
}
