
class foo
{
  void bar(foo&& f)
  {
    foo(static_cast<foo&&>(f));
    //foo(std::move(f));
  }
};

int main()
{
}

