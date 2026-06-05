
struct A {
  void operator()(int) {}
};
inline constexpr struct {
  constexpr auto operator()(A fn) const
  {
    return [f{fn}](auto &&tpl) 
        noexcept(noexcept(&f))
    { return true; };
  }
} error;

inline constexpr struct {
  constexpr auto operator()(A fn) const
  {
    return [f{fn}](auto &&tpl) 
        // noexcept(noexcept(&f))
    { return true; };
  }
} no_error;



int main() {
  (void)error( A{} )(1);    // $1
  (void)no_error( A{} )(1); // $2

}