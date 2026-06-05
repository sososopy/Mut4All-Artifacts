
    #include <initializer_list>

    template <class T, class K = long long int>
    struct C
    {
        C<T, K>(std::initializer_list<T> L) {}
    };
    
    int main()
    {
        C<int> Cont = {1, 2};
    }

