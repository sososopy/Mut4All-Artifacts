

template<class...XS>
auto List(XS...xs)
{
  return [=](auto processList){return processList(xs...);};
};

auto l1 = List(42);

auto foo = [](auto... xs1)
  {
    return [=]()
    { 
      return l1([=](auto)
      {
        return __builtin_printf("%d",xs1...);
      });  
    };
  };

int main()
{
  auto concat = l1(foo);
  concat();
  __builtin_printf("\n");
}


