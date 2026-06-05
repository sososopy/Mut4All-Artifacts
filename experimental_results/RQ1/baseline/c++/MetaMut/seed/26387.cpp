

class Base {
  int i;
};

template <auto V>
struct Foo {
  int a;
};


int main()
{
    struct Foo<&Base::i> struct_foo;
   
}
