
$ cat main.cpp
template <typename T>
class C
{
    void foo();
};

C<int> g_c_int;
C<double> g_c_double;

template <typename T>
void C<T>::foo() {}

template class __attribute__((visibility("default"))) C<int>;
template class C<double>;


