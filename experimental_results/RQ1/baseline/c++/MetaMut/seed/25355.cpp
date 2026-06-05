
struct OK {
  char no_padding = 0;
  constexpr OK() {}
};
static_assert((OK{} = OK{}, true), "");

struct T {
  alignas(2) char with_padding = 0;
  constexpr T() {}
};
// emits-error {{non-constant condition in static assert}}
// emits-error {{error accessing value of '<anonymous>' through a 'char [1]' glvalue in a constant expression}}
static_assert((T{} = T{}, true), "");
