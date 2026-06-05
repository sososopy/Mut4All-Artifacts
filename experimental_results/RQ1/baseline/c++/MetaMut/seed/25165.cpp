
   template <class V> struct tt{
     using ut = tt<V>;
     ut& operator++();
};
template <class V> tt<V>& tt<V>::operator++() {}

