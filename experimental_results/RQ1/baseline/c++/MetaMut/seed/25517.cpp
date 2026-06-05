
namespace bar
{
  void f(char x){;}
}

namespace foo
{
  // If the using clause is here; no error	
  // using bar::f;
  template <typename T> void f(T x);  
  template <> void f(int x);
  
  // With the using clause is here; error
  using bar::f;
  
  template <typename T> void f(T x){; }  
  template <> void f(int x){ ; }
}

