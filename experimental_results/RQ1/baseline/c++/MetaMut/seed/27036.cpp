
template <class _Tp> struct remove_reference        {typedef _Tp type;};
template <class _Tp> struct remove_reference<_Tp&>  {typedef _Tp type;};

typedef int __attribute__((__may_alias__)) int_a;

typedef remove_reference<int&>::type t;
typedef remove_reference<int_a&>::type q;

int main() {
    return 0;
}



