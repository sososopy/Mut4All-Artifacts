
template <typename T>
struct wrapper final
{
  T value;
  template <typename ...Args>
  wrapper(Args &&...args)
    : value(args...)
  {
  }
};

struct non_copyable
{
  non_copyable(const non_copyable &) = delete;
  non_copyable();
  ~non_copyable();
};

template <typename T>
class thread_local_variable final
{
private:
  wrapper<T> *pointer;
public:
  template <typename ...Args>
  thread_local_variable(Args &&...args)
  {
    wrapper<T> v(args...);
    pointer = &v;
  }
  T &get()
  {
    return pointer->value;
  }
};

non_copyable &fn()
{
  thread_local_variable<non_copyable> v;
  return v.get();
}


