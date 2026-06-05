
struct default_check
{ 
  template<typename U> static int check(...);
};

struct is_addable : default_check
{
  using default_check::check; // #1
  using type = decltype(check<void>(0));
};

int main() {}

