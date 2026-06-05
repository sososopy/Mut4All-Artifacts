

module;
# 1 "" 1 3
struct Trans_NS__V2_error_category {
  virtual equivalent();
};
struct error_condition {
  Trans_NS__V2_error_category &category() const;
};
struct error_category {
  friend Trans_NS__V2_error_category const &to_std_category();
};
template <typename> struct _Rb_tree_iterator {
  typedef _Rb_tree_iterator _Self;
  friend operator==(_Self, _Self) {}
};
template <typename _Val> struct _Rb_tree {
  typedef _Rb_tree_iterator<_Val> iterator;
};
template <typename _Key, typename> struct map {
  typedef typename _Rb_tree<_Key>::iterator iterator;
};
struct std_category {
  bool equivalent(const error_condition &) const;
};
map<const error_category *, int>::iterator __trans_tmp_2;
inline Trans_NS__V2_error_category const &to_std_category() {
  map<const error_category *, std_category>::iterator i;
  i == __trans_tmp_2;
}
inline bool std_category::equivalent(const error_condition &condition) const {
  dynamic_cast<std_category *>(&condition.category());
}
# 0 "" 2
export module foo;

