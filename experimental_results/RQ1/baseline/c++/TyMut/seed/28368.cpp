
#define ERR_MAX -20

#define s_errmsg(r)          \
        _k_errmsg[(((int)r) <= 0 && ((int)r) > ERR_MAX) ? -(r) : -ERR_MAX]

extern const char *_k_errmsg[];

template <int size>
inline int DoFoo() {
  int __ret = 0;
  const char *n = s_errmsg(__ret);
  return __ret;
}
