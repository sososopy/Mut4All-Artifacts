
template<class T> struct A{
        typedef T& Type; // #0
};
template<class T> void f(const typename A<T>::Type){}  // #1
template <> void f<int>(const typename A<int>::Type){} // #2
 
