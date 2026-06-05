
template <typename T>
concept bool Any()
{
  return true;
}

struct my_struct
{
  template <Any... Args>
  auto sample(Args... args) -> void;
};

int main()
{
  my_struct{}.sample();
}
