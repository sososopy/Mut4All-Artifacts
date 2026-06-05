
template <typename> class a;
class b;
template <> class a<b> {
public:
  enum class c { d };
  using enum c;
};
class b : public a<b> {};
template <int> class e { char g = (char)b::d; };
e<0> f;

