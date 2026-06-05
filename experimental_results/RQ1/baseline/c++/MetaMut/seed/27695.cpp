
template<auto... T_values>
concept C = true;

int main()
{
    auto b = C<1, 2>;  // ok
    if (C<1, 2>);      // error
}

