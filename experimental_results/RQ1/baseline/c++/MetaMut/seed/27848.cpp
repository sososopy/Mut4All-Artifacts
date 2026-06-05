
namespace std {
class type_info {};
} // namespace std
void fn1() { !((void **)&typeid(short))[0]; }

