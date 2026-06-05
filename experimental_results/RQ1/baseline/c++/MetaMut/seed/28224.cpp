

void f(unsigned char) = delete;
void f(int) { }

enum TEnum: unsigned char {
    Kot = 67
};

template<typename T> void g(const T& t) { f(t); }
template<> void g(const unsigned char&) { }

int main()
{
  g(Kot);
}


