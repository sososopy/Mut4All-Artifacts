
#define DIAGNOSTIC_HELPER0(x) #x
#define DIAGNOSTIC_HELPER1(kind, y) DIAGNOSTIC_HELPER0(GCC diagnostic kind #y)

#define DIAGNOSTIC(kind, warning, statements)\
    _Pragma("GCC diagnostic push")\
    _Pragma(DIAGNOSTIC_HELPER1(kind, warning))\
    statements \
    _Pragma("GCC diagnostic pop")

DIAGNOSTIC(error, -Wuninitialized,
int foo(int a, int b){return a;}
int main(){
    int i;
    return foo(i, i);
}
