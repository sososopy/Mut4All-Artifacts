
template <int>
struct L {};
template <int k>
template <typename T>
void L<k>::O<T>::Fun() {}
L<0>::O<int>::Fun();

