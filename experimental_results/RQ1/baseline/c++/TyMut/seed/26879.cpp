
template<class T>
void test(T b)
{
    const int a = b;
    [&] () { return a, a; }();
}

int main() {
    test(1);
 return 0;
}
