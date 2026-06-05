
namespace Foo
{
    int x;
   
    class Foo
    {
        public:
        static int x;
    };
   
    int Foo::x;
}


int main()
{
   using namespace Foo;
   Foo::x;
   return 0;
} 
