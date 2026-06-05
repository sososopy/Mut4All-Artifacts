template<typename T>
struct S
{
  S(T, T) { }
};

char* begin();
char* end();

void
test01()
{
  S s(begin(). end());  // should be comma not dot
}

