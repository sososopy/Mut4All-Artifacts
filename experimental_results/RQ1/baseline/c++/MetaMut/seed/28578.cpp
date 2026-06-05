
template<class T>
int foo(T);

template int foo(int);

template<class T>
int foo(T) = delete;
