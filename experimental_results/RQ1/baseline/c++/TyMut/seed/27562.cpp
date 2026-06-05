
#define LIKELY(x) __builtin_expect(!!(x), 1)

class Code
{
public:
  constexpr bool operator==(Code rhs) const noexcept { return value_ == rhs.value_; }

private:
  int value_;
};

int func(const Code a, const Code b, bool cond)
{
  return LIKELY(a == b || cond) ? 0 : 1;
}
