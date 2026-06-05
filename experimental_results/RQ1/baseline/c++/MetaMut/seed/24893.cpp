
struct add_fn
{
    template <typename Self>
    Self operator++(this Self&& self, int)
    {
      auto temp = self;
      ++self;
      return temp;
    }
};

struct A : add_fn 
{
    int n;
    A(int n) : n(n) {}

    A& operator++() 
    {
      ++n;
      return *this;
    }

    // this doesn't work either:
    // A& operator++(this A& self)
    // {
    //   ++self.n;
    //   return self;
    // }
};

int main()
{
    A a { 5 };
    ++a; // ok
    a++; // error: no 'operator++(int)' declared for postfix '++'
}

