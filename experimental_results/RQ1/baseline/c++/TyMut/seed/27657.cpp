
template<typename T>
class Demo
{
  struct
  {
    Demo* p;
  } payload{this};
  friend decltype(payload);
};

int main()
{
  Demo<int> d;
}

