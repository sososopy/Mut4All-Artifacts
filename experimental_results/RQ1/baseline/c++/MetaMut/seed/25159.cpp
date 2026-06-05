

template <typename, typename = int> class A;
struct B {
  int len;
  char name[];
};
template <class> struct C;
template <typename> struct D;
template <typename T, int N> struct D<T[N]>;
template <class T> struct F { typename D<T>::type type; };
template <class> struct G;
class H {
  template <class Source>
  H(Source &, typename C<G<typename F<Source>::type>>::type = 0);
};
template <class Char, class Traits> void operator<<(A<Char, Traits>, H);
class ErrnoLogMessage {
public:
  A<char> &stream();
};
B *inotify_handler_ievent {
ErrnoLogMessage().stream() << inotify_handler_ievent->name


