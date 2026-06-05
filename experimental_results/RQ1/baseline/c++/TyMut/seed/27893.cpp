
constexpr const char* c = reinterpret_cast<const char*>(0x123);
//---


//----------------
struct X {
 X* operator&();
};

X x[2];

const bool p = (reinterpret_cast<X*>(&reinterpret_cast<char&>(x[1]))
- reinterpret_cast<X*>(&reinterpret_cast<char&>(x[0]))) == sizeof(X);

enum E { e = p }; // e should have a value equal to 1
//----------------

