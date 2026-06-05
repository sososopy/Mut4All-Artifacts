
template <typename> struct AbstractTemplate {
  virtual ~AbstractTemplate() = 0;
};
static_assert(!__is_constructible(AbstractTemplate<int>), "");

