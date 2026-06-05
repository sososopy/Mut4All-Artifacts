enum class E : int { prio = 666 };
void f (int) __attribute__((constructor(E::prio)));
