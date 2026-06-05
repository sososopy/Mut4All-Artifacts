
template<int VALUE>
struct Outer
{
  struct StupidValueTrick
  {
    template<int VAL> friend struct Outer::StupidValueTrick;
  };
};
typedef Outer<42>::StupidValueTrick GoodValue;
GoodValue good;
