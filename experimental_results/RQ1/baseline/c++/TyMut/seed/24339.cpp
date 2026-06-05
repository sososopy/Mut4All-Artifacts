template<typename> struct foo 
{ 
    static_assert(noexcept(((foo *)1)->~foo()), ""); 
}; 

template class foo<int>;

