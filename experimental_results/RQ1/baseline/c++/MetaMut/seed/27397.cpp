
struct TestSkip {
    static int unused;
};
template <class T>
struct FIELD {
    TestSkip *obj;
    decltype(auto) value()  {  return (obj->unused);  }
};
int main()
{
    TestSkip s;
    int sf;
    int i;
    decltype((FIELD<int>().value())) b;
}

