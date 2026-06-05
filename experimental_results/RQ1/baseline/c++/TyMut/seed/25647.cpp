

namespace outer {
  template<typename> class A;
  namespace details {
    template<typename> class B
    {
      template<typename> friend class A;
    };
  }
}

template class outer::details::B must not befriend (but does to with gcc) template class outer::A, because the latter is not member of the innermost inclosing namespace. In order to befriend outer::A, the friend declaration would need to be qualified, i.e. template<typename> friend class outer::A;

