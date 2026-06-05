
struct X {
  template <unsigned N>
  struct Y {
    template <typename... Ts>
    Y(Ts...) {}
  };

  template <typename... Ts>
  Y(Ts...) -> Y<sizeof...(Ts)>;
};
