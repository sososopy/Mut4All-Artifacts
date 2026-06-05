
template <typename T>
T f (T, char*);

template <typename T>
decltype (f (T (), "")) g (T) { }

void h () { g (0); }

