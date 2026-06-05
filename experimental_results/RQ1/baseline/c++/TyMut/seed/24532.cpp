
struct A
{
  template<typename T> A(T) { }
};

template<typename T>
void
stuff(A = []{ }) {
}

int main() {
  stuff<int>();
  stuff<unsigned int>();
}
