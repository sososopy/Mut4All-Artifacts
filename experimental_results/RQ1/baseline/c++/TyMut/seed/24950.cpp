
template<typename T>
concept bool C1 = true;
template<typename T>
concept bool C2 = requires(C1 c1) {};

