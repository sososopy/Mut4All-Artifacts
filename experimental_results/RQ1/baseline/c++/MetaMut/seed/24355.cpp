
namespace {

  template <typename T>
  class A {
  public:

    template < typename U> A&& operator << ( U const & ) && {
      std::cerr << "... && version\n";
      return std::move(*this);
    }

    template <typename U> A& operator << ( U const &) & {
      std::cerr << "... &  version\n";
      return *this;
    }

  };


  template<>
  template<typename U>
  A<int>& A<int>::operator<< ( U const & ) & {
    std::cerr << "... &  version (int)\n";
    return *this;
  }

}


int main() {

  A<double>() << "";
  A<double> adouble;
  adouble << "";

  A<int>() << "";
  A<int> aint;
  aint << "";

}
