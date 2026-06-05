

template<typename T> struct shared_ptr { };

template<typename T, typename Arg>
shared_ptr<T> make_shared(Arg) { return {}; }

auto f(int n){
  return make_shared<int[n]>(1);
}
