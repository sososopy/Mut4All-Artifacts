
template <typename T> class A
{
    template <typename... Types> class B
    {
        B(const A<T>::B<Types...>&) = default;
    };
};

