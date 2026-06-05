
struct Bar {
  int a;
};
struct Foo{
  Foo(Bar arg) : bar(arg) {}
  Bar & bar;
};
int main() {
  Bar k;
  Foo oops(k);
  return 0;
}
