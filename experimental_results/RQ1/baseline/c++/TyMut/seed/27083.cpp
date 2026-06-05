
struct base
{
   void foo(){};
};

template < typename >
struct derived : base
{
   void foo()
   {
      auto l = [this](){base::foo();};
   // workaround: 
   // auto l = [this](){this->base::foo();};
   };
};

int main()
{
   derived<int> d;
   d.foo();
}
