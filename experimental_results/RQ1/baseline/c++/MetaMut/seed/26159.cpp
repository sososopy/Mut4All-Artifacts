
template<class T> struct U {
  int t = 0;
};

template<class T>
struct V { 
  U<T> size() const { return U<T>{}; }
};

template<typename T>
void Do() {
  V<int> v{};
  [=] { v.size(); };
}

int main() {

}
