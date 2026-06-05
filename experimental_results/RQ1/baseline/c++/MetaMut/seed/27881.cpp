
namespace {
  template<typename F>
    void run(F f, int i)
    {
      f(i);
    }
}

void f()
{
  run([](int) { }, 1);
}

