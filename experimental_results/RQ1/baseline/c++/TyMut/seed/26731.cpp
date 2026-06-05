void make_item()
{
  static int i{100};

  struct
  {
    int id{i++};
  } item;
}

int main()
{
  make_item<0>();
}

