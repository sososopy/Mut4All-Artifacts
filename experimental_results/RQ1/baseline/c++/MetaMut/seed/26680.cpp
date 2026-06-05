

template <int i> constexpr int find_longest_name()
{
  return sizeof("Main") - 1;
}

template <int i, int l = find_longest_name<i>()> void create_all_loggers()
{}

int main()
{
  create_all_loggers<1>();
}


