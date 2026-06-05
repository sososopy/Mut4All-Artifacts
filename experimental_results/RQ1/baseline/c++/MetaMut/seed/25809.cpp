

template <typename A>
class Base {
public:
    struct Data {
        unsigned int *ptr;
    };
};

template<typename A>
class Derived
    : private Base<A>
{
    using typename Base<A>::Data;
    Derived()
        : ptr(0)
    { }
public:
    const
#ifdef PACIFY_CLANG
    typename Base<A>::Data
#else 
    Data
#endif
    * get_ptr() const;
private:
    const Data *ptr;
};


template<typename A>
#if PACIFY_CLANG_ANGER_GCC
const typename Derived<A>::Data *
#else
const typename Base<A>::Data *
#endif 
Derived<A>::get_ptr() const 
{
    return ptr;
}
