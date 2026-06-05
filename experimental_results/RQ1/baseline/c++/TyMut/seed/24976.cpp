

struct function_impl_base {
  virtual ~function_impl_base() = default;
  virtual void invoke() = 0;
};

template<typename F>
struct function_impl : function_impl_base {
  function_impl(F f) : f(f) { }
  void invoke() { f(); }
  F f;
};

struct function
{
  template<typename F>
    function(F f) : impl(new function_impl<F>{f}) { }
  ~function() { delete impl; }
  function(const function&) = delete;
  function_impl_base* impl;
  void operator()() { impl->invoke(); }
};

template <class U>
struct LambdaHolder {
    LambdaHolder() : lambda_{[this]() { delete this; }} {
    }
    void Run() {
        lambda_();
    }
    function lambda_;
};

int main() {
    LambdaHolder<int>* l = new LambdaHolder<int>();
    l->Run();
}


