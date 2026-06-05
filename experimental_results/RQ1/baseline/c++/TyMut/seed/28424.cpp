
struct Base
{
   int b = 0;
};

struct Derived : Base
{
   int d = 2;

   constexpr bool foo(int x)
   {
      return x > d;
   }
};

int main()
{
   constexpr auto test = []()
   {
      struct Dummy
      {
         Derived derived;
      };

      Dummy d;
      Base & base = d.derived;
      static_cast<Derived &>(base).foo(1);

      // Derived d;
      // Base & base = d;
      // static_cast<Derived &>(base).foo(1);

      return true;
   }();

   return test;
}

