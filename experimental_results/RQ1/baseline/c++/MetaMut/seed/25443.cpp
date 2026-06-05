

template <class T> concept bool Large = sizeof(T) > 1;

template <class... T>
struct X
{
    X() requires Large<T> = default;
    X() requires !Large<T> = default;
};

int main()
{
    X<int> x;
    X<char> y;
}

