
class f {
    static int mVar;
    int g(int x) { int mVar=3; return x+mVar; }
};
int f::mVar = 1;

