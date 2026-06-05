template <class A>
concept always_true = true;

struct receiver {
  template <class Tag = int>
  friend void tag_invoke( always_true auto, receiver self) {
  }
};