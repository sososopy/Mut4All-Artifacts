

struct Type
{
  void get() const& { }
  void get() const&& { }
};

int main()
{
  Type{}.get();
}

