
template<class> concept c = true;
template<class T>
struct ts
{
  template<class T1>
   requires(c<T1>)
    ts(const ts<T1>&);
  template<class T1>
   requires(!c<T1>)
    ts(const ts<T1>&) = delete;
  ts() = default;
};


using P = ts<int>;
void (*f)(P);
template<class>
void f1()
{  
  P x;
  f(x);
}
