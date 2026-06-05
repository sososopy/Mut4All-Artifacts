
struct Option {
  virtual ~Option() {}
};
template <class DataType> class opt : public Option {};
template class opt<int>;

