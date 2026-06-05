
class Time { };
struct PasswordStore {
    void RemoveLogins(Time delete_begin)
{ __builtin_exit (0); }
};
template <typename g>
struct A {
void TestBody() {
    PasswordStore base_store ;
    base_store.RemoveLogins(Time::Time());
}
};
int main(void)
{
  A<int> a;
  a.TestBody();
  __builtin_abort ();
}


