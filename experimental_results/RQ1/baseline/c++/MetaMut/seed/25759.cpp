
struct Foo {};
void func()
{
  auto a = [](bool arg){
    if( arg )
      return Foo();
    return (const Foo)Foo();
  };
}
