template <class T>
class BadTuple {
  constexpr bool operator==(const BadTuple&) const;
};
template<class T>
constexpr bool BadTuple<T>::operator==(const BadTuple<T>&) const = default;

BadTuple<int> a;

