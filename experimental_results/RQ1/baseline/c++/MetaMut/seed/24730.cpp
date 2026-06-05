
template<class T, class ReturnType>
ReturnType f(T);

template<>
auto f(int)
{
    return 0;
}

