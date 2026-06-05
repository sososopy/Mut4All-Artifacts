
enum Foo
{
  a,
  b
};

inline Foo operator++(Foo &f, int) 
{
  return f = (Foo)(f + 1);
}

int main()
{
  int count = 0;
  for (Foo f = a; f <= b; f++) {
    count++;
  }
  return count;
}
