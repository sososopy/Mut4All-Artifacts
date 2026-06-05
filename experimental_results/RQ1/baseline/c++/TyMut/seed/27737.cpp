
namespace {}
namespace a {
void isgreater();
void isgreater(double);
template <typename> void isgreater();
bool isgreaterequal();
template <typename> void isgreaterequal();
} // namespace a
using a::isgreater;
using a::isgreaterequal;

