
template<class T>
struct X{};

void f(X<int>(&&)[1])
{}
int main()
{
  f({X<int>()});
}

