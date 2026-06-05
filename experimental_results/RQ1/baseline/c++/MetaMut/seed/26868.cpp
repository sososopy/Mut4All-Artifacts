
struct ostream {} cout; 
template<typename T> struct A{ T t; };
struct B{};

namespace {
template<typename T>
ostream& operator<< (ostream& out, const A<T>&v) 
  { return out << v.t; }

ostream& operator<< (ostream& out, const B&) 
  { return out; }
}

int main(){
  A<B> a;
  cout << a; 
}
