
export module mod;

export template<class N> struct quantity {
  template<class N2>
  explicit(requires { 0; }) operator quantity<N2>() const;
};

import mod;
int main() {
  return requires { (quantity<double>)quantity<int>{}; };
}
