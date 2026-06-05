template <typename, void()> class ScopedPtr;
template <typename T, void(&Destroyer)()> ScopedPtr < T, Destroyer> a;
