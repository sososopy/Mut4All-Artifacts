
constexpr auto foo(int i)
{
    if(i == 0) {
        return 0;
    } else {
        return 'a';
    }
}

int main()
{
    using X = decltype( foo(0) );
    return X {};
}
