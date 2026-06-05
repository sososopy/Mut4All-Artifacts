
template<class... Ts>
void f(Ts...) {}

void(*pfi)(int) = &f<int>; // OK
void(*pfci)(int) = &f<const int>; // error

void(*pcfi)(const int) = &f<int>; // OK
void(*pcfci)(const int) = &f<const int>; // error

