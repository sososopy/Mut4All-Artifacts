
template<class T>
concept C = true;
auto x = auto(1);     // valid (P0849R8)
auto y = C auto(1);   // expect a error

