
template <typename> struct TransparentSupport {
  template <typename> using key_arg = int;
};
template <typename Key> struct Map {
  template <typename LookupKey>
  using key_arg = typename TransparentSupport<Key>::template key_arg<LookupKey>;
};
struct MapKey {
  Map<MapKey> map_;
};

