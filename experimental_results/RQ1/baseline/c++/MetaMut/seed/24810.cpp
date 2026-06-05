
using type = void;
template <typename T> void checkSwap(T &t) noexcept(noexcept(t));
void qSwap(int value1) noexcept(noexcept(checkSwap(value1)));
type callDestructorIfNecessary(int &) noexcept {}
