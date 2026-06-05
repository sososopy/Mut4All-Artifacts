
template <typename... Ts>
void foo(Ts... xs)
{
    [...xs...]{}();
}

int main()
{
    foo(0, 1, 2);
}
