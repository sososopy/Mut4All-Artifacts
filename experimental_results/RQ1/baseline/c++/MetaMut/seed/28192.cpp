
struct z8 {
  constexpr static int qq /* = 0 */;
};

template<typename T>
struct kf {
  kf (const kf &) noexcept (T::qq);
};

struct lk {
  kf<z8> e1;
};

template<typename T>
T &sc ();

struct b6 {
  decltype (lk (sc<lk> ())) zz;
};

