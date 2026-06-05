
template <typename T> struct foo { foo(); };
template <typename T> foo<T>::foo() {}
template class foo<int>;

