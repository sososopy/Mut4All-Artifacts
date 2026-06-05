
template<typename T> void f(int a[sizeof(static_cast<int &>( T() ))]);
void g() { f<double[42]>(0); }
