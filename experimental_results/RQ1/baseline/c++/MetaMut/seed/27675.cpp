
template <typename b, typename c> concept d = requires(b e) { e.operator c; };
template <typename f, typename g> requires(d<f, g>) bool equal(f, g);
template <typename h> struct i {
  i(h);
  operator h();
};
bool fun() {
  i a(2.0f);
  return equal(a, 3.0f);
}

