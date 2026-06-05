
struct range_t {  double init;  };
template<range_t setup> struct knob_t {
  int value = setup.init;
};
struct Helpers
{
  knob_t<range_t{100}> inputs;
};
template <class T, int N>  auto detect_fields_count_dispatch() noexcept -> decltype(sizeof(T{}));
int t = detect_fields_count_dispatch<Helpers>();

