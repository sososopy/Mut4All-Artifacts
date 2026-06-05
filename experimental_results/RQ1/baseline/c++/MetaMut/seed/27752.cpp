template <typename T> struct C {
  friend C(T::fn)();  // not implicit typename context, declarator-id of friend
                      // declaration
};

