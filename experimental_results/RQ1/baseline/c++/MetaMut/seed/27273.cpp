struct X {
  void f() { }
};

template<typename T>
requires (sizeof(T)==1)
void f(T)
{ }

int main()
{
  f(&X::f);
}

