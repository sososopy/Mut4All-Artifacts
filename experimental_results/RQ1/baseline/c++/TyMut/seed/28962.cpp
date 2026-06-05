

#include <vector>
#include <string>
#include <coroutine>

struct dummy
{
  struct promise_type
  {
    std::suspend_always initial_suspend() noexcept;
    std::suspend_always final_suspend() noexcept;
    void unhandled_exception() noexcept;
    void get_return_object() noexcept;
  };

  bool await_ready() const noexcept;
  template <class U> void await_suspend(U);
  void await_resume();
};

dummy g1(std::vector<int>);
dummy f1() { co_await g1({}); } // no ICE

dummy g2(std::vector<std::string>);
dummy f2() { co_await g2({{}}); } // ICE
