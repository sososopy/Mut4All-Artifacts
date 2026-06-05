
template <typename> struct a;
template <bool> struct b;
template <typename> class c {
  template <typename f> void d(f e, typename b<a<decltype(e)>::h>::g);
public:
  static bool h;
};
bool h = c<int>::h;

