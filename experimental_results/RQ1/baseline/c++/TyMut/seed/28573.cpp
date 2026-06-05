
template <bool> struct enable_if;
class function {
public:
  template <typename _Functor> void operator=(_Functor);
};
class map {
public:
  function operator[](int);
};
enum { avc_es };
template <typename> void do_probe();
template <typename Treader>
typename enable_if<Treader ::value>::type do_probe();
void prober_for_type() {
  map type_probe_map;
  type_probe_map[avc_es] = do_probe<int>;
}

