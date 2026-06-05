
template <typename a> concept HasMapInterface = requires(a t) { t.cend; };
template <typename Mapper>
requires HasMapInterface<decltype(Mapper::map())> struct l {};
struct mymap {
  int cend();
};
struct p {
  static mymap map();
};
l<p> q;
