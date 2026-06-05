
namespace N {
  template<class, class> concept C = true;
}

struct X {
  N::C<int> auto f() { return 0; }
};
