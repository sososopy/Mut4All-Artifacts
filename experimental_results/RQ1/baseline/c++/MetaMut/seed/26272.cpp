
template<typename T> using A = T*;

template<typename T, int = A<T>::X> struct B;
