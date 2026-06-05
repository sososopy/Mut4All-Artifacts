
#include <future>
#include <iostream>
#include <thread>
#include <vector>

template <class T> T &self(T &t) { return t;  }
template<typename T> struct Dependent {  };

template<typename T>
class Synchronised : Dependent<T>{
 public:
  explicit Synchronised(T t = T()) : t_(t) {}
  template<typename Functor>
  auto operator()(Functor functor) const ->decltype(functor(self(*this).t_)) {
  //auto operator()(Functor functor) const ->decltype(functor(this->t_)) {
    std::lock_guard<std::mutex> lock(mutex_);
    return functor(t_);
  }
 private:
  mutable T t_;
  mutable std::mutex mutex_;
};


int main() {

    Synchronised<std::string> sync_string("Start\n");
    std::vector<std::future<void>> futures;
}