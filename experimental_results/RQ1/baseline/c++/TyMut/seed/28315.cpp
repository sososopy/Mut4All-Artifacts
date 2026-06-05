
struct a
{
  using get = void;
};

struct b : a
{
  int get(int i) const;
};

template <typename T>
concept c = requires { typename T::get; };

int main()
{
  return c<b>;
}

