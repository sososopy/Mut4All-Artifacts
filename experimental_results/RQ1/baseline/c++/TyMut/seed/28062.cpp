template<integral T>  // oops, should be std::integral
struct ref_view
{
  ref_view(T) { }
};

ref_view r{1};

