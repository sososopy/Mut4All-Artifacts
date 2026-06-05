
  #include <boost/stl_interfaces/iterator_interface.hpp>

  template<typename T>
  struct iterator : boost::stl_interfaces::iterator_interface<iterator<T>, 
  std::forward_iterator_tag, T>
  {
    using base_type = boost::stl_interfaces::iterator_interface<iterator<T>, std::forward_iterator_tag, T>;
    using base_type::operator++;

    T& operator*() const;
    iterator& operator++();
    bool operator==(iterator) const;
  };

  static_assert(std::incrementable<iterator<int>>);

