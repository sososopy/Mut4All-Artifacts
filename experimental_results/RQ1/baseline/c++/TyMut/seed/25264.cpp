

template <class> struct App;
template <class> struct Dyn {
  typedef typename Dyn::Acc Acc;
  App<Acc> a;
};
