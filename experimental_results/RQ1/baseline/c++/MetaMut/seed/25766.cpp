
struct Exception {
  Exception(int) {}
};

template<typename Arg>
int make_shared(Arg arg)
{
  Exception* p = arg; p = p;
  return 0;
}

struct InvalidArgumentException : public virtual Exception
{
 explicit InvalidArgumentException() : Exception(make_shared(this))
 { }
  
  virtual ~InvalidArgumentException() noexcept {}
};

int main()
{
  throw InvalidArgumentException();
}
