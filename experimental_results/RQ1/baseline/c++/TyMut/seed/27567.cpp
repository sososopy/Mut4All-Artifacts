
template <typename> constexpr bool trait = true;
template <typename T> concept C = trait<T>;

void f(auto) {
  []<typename T>(){
    requires { C<T>; };
  };
}

int main() { f(42); }

